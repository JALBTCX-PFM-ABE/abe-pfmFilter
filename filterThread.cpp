
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/



#include "filterThread.hpp"

filterThread::filterThread (QObject *parent)
  : QThread(parent)
{
}



filterThread::~filterThread ()
{
}



void filterThread::filter (OPTIONS *op, int32_t ph, PFM_OPEN_ARGS *oa, int32_t sr, int32_t er, int32_t o, int32_t p)
{
  QMutexLocker locker (&mutex);

  l_options = op;
  l_pfm_handle = ph;
  l_open_args = *oa;
  l_start_row = sr;
  l_end_row = er;
  l_overlap = o;
  l_pass = p;

  if (!isRunning ()) start ();
}



void filterThread::run ()
{
  int32_t             percent = 0, old_percent = -1;
  QString             string;


  mutex.lock ();

  OPTIONS *options = l_options;
  int32_t pfm_handle = l_pfm_handle;
  PFM_OPEN_ARGS open_args = l_open_args;
  int32_t start_row = l_start_row;
  int32_t end_row = l_end_row;
  int32_t overlap = l_overlap;
  int32_t pass = l_pass;

  mutex.unlock ();


  //qDebug () << __LINE__ << pass;



  BIN_RECORD bin;
  DEPTH_RECORD *depth;
  POS_DATA *pos = NULL;
  int32_t recnum;
  int32_t count = 0;
  uint8_t multi_line = NVFalse, multi_line_in_hpc = NVFalse, quota = NVFalse;
  float datum_offset = options->datum_offset, cutoff = 60.0 + options->hpHeight, load_cutoff = cutoff + 20.0;


  if (options->datum_shifted) datum_offset = 0.0;


  for (int32_t i = start_row ; i <= end_row ; i++)
    {
      NV_I32_COORD2 ref_coord;

      ref_coord.y = i;

      int32_t i_start = qMax (i - overlap, 0);
      int32_t i_end = qMin (i + overlap, open_args.head.bin_height - 1);

      for (int32_t j = 0 ; j < open_args.head.bin_width ; j++)
        {
          ref_coord.x = j;
          uint8_t modified = NVFalse;


          count = 0;

          int32_t j_start = qMax (j - overlap, 0);
          int32_t j_end = qMin (j + overlap, open_args.head.bin_width - 1);


          //  Load the point array.

          for (int32_t k = i_start ; k <= i_end ; k++)
            {
              NV_I32_COORD2 coord;

              coord.y = k;


              for (int32_t m = j_start ; m <= j_end ; m++)
                {
                  coord.x = m;

                  read_bin_record_index (pfm_handle, coord, &bin);


                  //  Don't load data where the average depth is greater than ~80 meters.

                  if (bin.num_soundings && (bin.avg_filtered_depth + datum_offset) <= load_cutoff)
                    {
                      if (!read_depth_array_index (pfm_handle, coord, &depth, &recnum))
                        {
                          for (int32_t n = 0 ; n < recnum ; n++)
                            {
                              if (!(depth[n].validity & (PFM_INVAL | PFM_DELETED | PFM_REFERENCE)))
                                {
                                  if (depth[n].attr[2] == 41) QPRINT depth[n].validity;

                                  pos = (POS_DATA *) realloc (pos, sizeof (POS_DATA) * (count + 1));

                                  if (pos == NULL)
                                    {
                                      perror ("Allocating pos array in filterThread.cpp");
                                      exit (-1);
                                    }


                                  //  Populate the position array with the position of the point in meters from the southwest corner of the area.
                                  //  This allows us to directly compute distance between points using just the sqrt function.

                                  double dist_x, dist_y;

                                  pfm_geo_distance (pfm_handle, open_args.head.mbr.min_y, depth[n].xyz.x, depth[n].xyz.y, depth[n].xyz.x, &dist_y);
                                  pfm_geo_distance (pfm_handle, depth[n].xyz.y, open_args.head.mbr.min_x, depth[n].xyz.y, depth[n].xyz.x, &dist_x);

                                  pos[count].x = (float) dist_x;
                                  pos[count].y = (float) dist_y;
                                  pos[count].z = depth[n].xyz.z + datum_offset;
                                  pos[count].h = depth[n].horizontal_error;
                                  pos[count].v = depth[n].vertical_error;
                                  pos[count].coord = coord;
                                  pos[count].array_index = n;
                                  pos[count].line = depth[n].line_number;
                                  pos[count].count = 0;
                                  pos[count].killed = NVFalse;
                                  count++;
                                }
                            }

                          free (depth);
                        }
                    }
                }
            }


          //  If we don't have a count, we don't have any data.

          if (count)
            {
              float hp_half_height = options->hpHeight / 2.0;


              //  Determine which points need to be deleted.  This uses the dreaded Hockey Puck of Confidence (TM).  We have to
              //  run through this process iteratively since a point may be deemed good based on nearby points that might later
              //  be deemed invalid.  To do this we run the loop until we don't kill any more points.

              int32_t new_killed = 0;
              do
                {
                  new_killed = 0;
                  uint8_t inside_ref = NVFalse;

                  for (int32_t k = 0 ; k < count ; k++)
                    {
                      multi_line = NVFalse;
                      multi_line_in_hpc = NVFalse;
                      quota = NVFalse;


                      //  Check to see if this point is in our referenced bin.

                      if (pos[k].coord.x == ref_coord.x && pos[k].coord.y == ref_coord.y)
                        {
                          //  Set flag when we get a point inside our reference bin.

                          inside_ref = NVTrue;


                          //  Make sure we haven't already marked it for killing.  Also check against the cutoff depth.

                          if (!pos[k].killed && pos[k].z <= cutoff)
                            {
                              //  Increment the count of total points.

                              options->point_count[pass]++;


                              //  Check for inclusion and/or exclusion.

                              uint8_t filter_point = NVTrue;
                              if (options->in_polygon_count)
                                {
                                  if (!inside_polygon2 (options->in_polygon_x, options->in_polygon_y, options->in_polygon_count, pos[k].x, pos[k].y))
                                    filter_point = NVFalse;
                                }

                              if (options->ex_polygon_count)
                                {
                                  if (inside_polygon2 (options->ex_polygon_x, options->ex_polygon_y, options->ex_polygon_count, pos[k].x, pos[k].y))
                                    filter_point = NVFalse;
                                }


                              if (filter_point)
                                {
                                  for (int32_t m = 0 ; m < count ; m++)
                                    {
                                      //  Don't check against itself.

                                      if (m != k)
                                        {
                                          //  Simple check for exceeding distance in X or Y direction (prior to a radius check).

                                          double diff_x = fabs (pos[k].x - pos[m].x);
                                          double diff_y = fabs (pos[k].y - pos[m].y);


                                          //  The radius.

                                          double dist = options->hpRadius;  // +  horizontal error  ???


                                          //  The depth offset.

                                          float height_offset = hp_half_height;  // + vertical error ???


                                          //  Simple check for either X or Y exceeding our distance (so we don't do too many SQRTs)

                                          if (diff_x <= dist && diff_y <= dist)
                                            {
                                              double pnt_dist = sqrt (diff_x * diff_x + diff_y * diff_y);


                                              //  Check the distance.

                                              if (pnt_dist <= dist)
                                                {
                                                  //  Use half the radius to check for multiline data.  The assumption being
                                                  //  that, if you have 200% coverage, you should have a point from a different line within half the
                                                  //  radius.  This is probably only true for LiDAR data but that is probably the only time you'll
                                                  //  require multiline.

                                                  if (pnt_dist <= options->hpRadius * 0.5 && pos[k].line != pos[m].line) multi_line = NVTrue;


                                                  //  Check the Z difference.

                                                  if (fabs (pos[k].z - pos[m].z) < height_offset)
                                                    {
                                                      if (options->hpNeighbors || (!options->hpNeighbors && multi_line && pos[k].line != pos[m].line))
                                                        {
                                                          //  Check for multi line inside the hockey puck.

                                                          if (multi_line && pos[k].line != pos[m].line) multi_line_in_hpc = NVTrue;


                                                          pos[k].count++;


                                                          //  If we've met our quota for validity let's go ahead and stop checking points.

                                                          if ((options->hpNeighbors && pos[k].count >= options->hpNeighbors) ||
                                                              (multi_line_in_hpc && pos[k].count >= options->hpMultiNeighbors))
                                                            {
                                                              quota = NVTrue;
                                                              break;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }


                                  //  If we don't meet the HP filter criteria, add this point to the filter kill list and set it's temporary invalid flag
                                  //  (killed) so we don't use it to validate other points.

                                  if (!quota)
                                    {
                                      //  If we're only trying to check data in 200% coverage areas we don't want to invalidate points in areas
                                      //  where there isn't 200% coverage.

                                      if (options->hpNeighbors || multi_line)
                                        {
                                          pos[k].killed = NVTrue;


                                          //  Add one to the while loop's kill count so we'll know when to stop.

                                          new_killed++;


                                          //  Increment the count of filtered points.

                                          options->filter_count[pass]++;
                                        }
                                    }
                                }
                            }
                        }


                      //  If we were inside the reference bin and now we've moved out, we can break the loop.

                      else
                        {
                          if (inside_ref) break;
                        }
                    }


                  uint8_t done = NVFalse;

                  for (int32_t k = 0 ; k < count ; k++)
                    {
                      //  Check to see if this point is in our referenced cell and see if needs to be marked as suspect.

                      if (pos[k].coord.x == ref_coord.x && pos[k].coord.y == ref_coord.y && pos[k].killed)
                        {
                          bin.coord = pos[k].coord;
                          read_bin_depth_array_index (pfm_handle, &bin, &depth);


                          //  Now we loop through the array until the coordinates change (since they were loaded by bin cell).

                          for (int32_t m = k ; m < count ; m++)
                            {
                              if (pos[m].coord.x != ref_coord.x || pos[m].coord.y != ref_coord.y)
                                {
                                  done = NVTrue;
                                  break;
                                }

                              if (pos[m].killed)
                                {
                                  if (options->invalidate)
                                    {
                                      depth[pos[m].array_index].validity |= (PFM_FILTER_INVAL | PFM_MODIFIED);
                                    }
                                  else
                                    {
                                      depth[pos[m].array_index].validity |= (PFM_SUSPECT | PFM_MODIFIED);
                                    }

                                  update_depth_record_index_ext_flags (pfm_handle, &depth[pos[m].array_index]);
                                  modified = NVTrue;
                                }
                            }

                          if (modified)
                            {
                              if (options->invalidate)
                                {
                                  recompute_bin_values_from_depth_index (pfm_handle, &bin, 0, depth);
                                }
                              else
                                {
                                  bin.validity |= PFM_SUSPECT;
                                  recompute_bin_values_from_depth_index (pfm_handle, &bin, PFM_SUSPECT, depth);
                                }

                              modified = NVFalse;
                            }

                          free (depth);
                          if (done) break;
                        }
                    }
                } while (new_killed);


              //  Free the memory we allocated.

              free (pos);
              pos = NULL;
            }


          percent = NINT (((float) ((i - start_row) * open_args.head.bin_width + j) /
                           (float) (open_args.head.bin_width * (end_row - start_row))) * 100.0);
          if (percent != old_percent)
            {
              emit percentValue (percent, pass);
              old_percent = percent;
            }
        }
    }


  emit percentValue (100, pass);


  //qDebug () << __LINE__ << filters_count << pass;


  emit completed (pass);
  qApp->processEvents ();
}
