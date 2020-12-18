
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



#ifndef PFMFILTERDEF_H
#define PFMFILTERDEF_H

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


#include "nvutility.h"
#include "nvutility.hpp"
#include "pfm.h"
#include "shapefil.h"


/*  Define this to do memory check debugging.  */

#undef __PFM_FILTER_DEBUG__


#define FILTER_THREADS    24


typedef struct
{
  double        hpRadius;
  double        hpHeight;
  int32_t       hpNeighbors;
  int32_t       hpMultiNeighbors;
  double        datum_offset;
  uint8_t       datum_shifted;
  uint8_t       invalidate;
  NV_F64_XYMBR  mbr;
  NV_F64_XYMBR  in_mbr;
  NV_F64_XYMBR  ex_mbr;
  int32_t       in_polygon_count;
  double        in_polygon_x[2000];
  double        in_polygon_y[2000];
  int32_t       ex_polygon_count;
  double        ex_polygon_x[2000];
  double        ex_polygon_y[2000];
  int32_t       point_count[FILTER_THREADS];
  int32_t       filter_count[FILTER_THREADS];
  int32_t       window_x;
  int32_t       window_y;
  int32_t       window_width;
  int32_t       window_height;
  uint8_t       replace;
  QString       input_dir;
  QString       area_dir;
  QString       exclude_dir;
  QFont         font;                       //  Font used for all ABE GUI applications
} OPTIONS;


typedef struct
{
  QGroupBox           *sbox;
  QProgressBar        *sbar;
  QListWidget         *list;
} RUN_PROGRESS;


#endif
