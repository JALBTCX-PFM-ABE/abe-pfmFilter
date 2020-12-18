
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



QString hpRadiusValueText = 
  optionPage::tr ("Change the X/Y <b>Radius</b> (in meters) to be used for the Hockey Puck filter.  Increasing this value will decrease the amount "
                  "of filtering.<br><br>"
                  "<b>IMPORTANT NOTE: The horizontal uncertainty of each of the two points being compared at any given time is added to the "
                  "horizontal radius so that any possible overlap of the positional data can be used to validate a point.</b>");

QString hpHeightValueText = 
  optionPage::tr ("Change the vertical <b>Height</b> (in meters) to be used for the Hockey Puck filter.  Increasing this value will decrease the amount "
                  "of filtering.");

QString hpNeighborsValueText = 
  optionPage::tr ("Change the number of <b>Neighbors</b> required within the horizontal <b>Radius</b> and vertical <b>Height</b> (i.e. the hockey puck) "
                  "of the Hockey Puck filter.  Increasing this value will increase the amount of filtering.  Setting this value to 0 will force the "
                  "filter to only consider points in areas with multiple lines (safer).");

QString hpMultiNeighborsValueText = 
  optionPage::tr ("Change the number of <b>Multi-line Neighbors</b> (i.e. neighbors when more than one line is present) required within the "
                  "horizontal <b>Radius</b> and vertical <b>Height</b> (i.e. the hockey puck) of the Hockey Puck filter.  Increasing this "
                  "value will increase the amount of filtering.");

QString datumText = 
  optionPage::tr ("Check this box if the data in the PFM is already shifted to a local datum (like MLLW or MSL).<br><br>"
                  "<b>IMPORTANT NOTE: This option is never saved from one run to the next because incorrectly setting it "
                  "will cause the program to improperly filter the data.</b>");

QString invText = 
  optionPage::tr ("Check this box if you wish to mark filtered data points as PFM_FILTER_INVAL instead of PFM_SUSPECT.<br><br>"
                  "<b>WARNING: This should only be done if you are absolutely sure that you will not be filtering out real, "
                  "hydrographic data!  This option should probably only be used for topographic LiDAR data.</b>");

QString offsetText = 
  optionPage::tr ("If the data in the PFM is not already shifted to a local datum (like MLLW or MSL) this value is added to the depth to shift "
                  "it to a reasonable approximation of the local datum.  If you don't have a value for this you can press the EGM08 button "
                  "to retrieve the EGM08 datum value at the center of the PFM.  This will shift it to a fair approximation of MSL.<br><br>"
                  "<b>IMPORTANT NOTE: This option is never saved from one run to the next because incorrectly setting it "
                  "will cause the program to improperly filter the data.</b>");

QString egmText = 
  optionPage::tr ("Click this button to retrieve the EGM08 datum offset from the EGM08 database (assuming you have it available).  This will be "
                  "placed in the <b>Datum offset</b> field and will be used to shift the data to an approximation of the MSL datum.");
