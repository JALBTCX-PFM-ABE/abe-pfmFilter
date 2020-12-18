
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



#ifndef FILTERTHREAD_H
#define FILTERTHREAD_H


#include "pfmFilterDef.hpp"


class filterThread:public QThread
{
  Q_OBJECT 


public:

  filterThread (QObject *parent = 0);
  ~filterThread ();

  void filter (OPTIONS *op = NULL, int32_t ph = -1, PFM_OPEN_ARGS *oa = NULL, int32_t sr = 0, int32_t er = 0, int32_t o = 0, int32_t pass = 0);


  typedef struct
  {
    float          x;
    float          y;
    float          z;
    float          h;
    float          v;
    NV_I32_COORD2  coord;
    int32_t        array_index;
    int32_t        line;
    int32_t        count;
    uint32_t       val;
    uint8_t        killed;
  } POS_DATA;


signals:

  void percentValue (int32_t percent, int32_t pass);
  void completed (int32_t pass);


protected:


  QMutex           mutex;

  OPTIONS          *l_options;

  PFM_OPEN_ARGS    l_open_args;

  int32_t          l_start_row, l_end_row, l_overlap, l_pass, l_pfm_handle;

  void             run ();


protected slots:

private:
};

#endif
