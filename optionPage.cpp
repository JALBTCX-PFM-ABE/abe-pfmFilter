
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



#include "optionPage.hpp"
#include "optionPageHelp.hpp"

optionPage::optionPage (QWidget *parent, OPTIONS *op):
  QWizardPage (parent)
{
  options = op;


  setTitle (tr ("Filter options"));

  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/pfmFilterWatermark.png"));


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->setMargin (5);
  vbox->setSpacing (5);
  QHBoxLayout *hbox = new QHBoxLayout;
  vbox->addLayout (hbox);


  QGroupBox *l40box = new QGroupBox (tr ("Options"), this);
  QHBoxLayout *l40boxLayout = new QHBoxLayout;
  l40box->setLayout (l40boxLayout);


  QGroupBox *rbox = new QGroupBox (tr ("Radius"), this);
  QHBoxLayout *rboxLayout = new QHBoxLayout;
  rbox->setLayout (rboxLayout);
  hpRadiusValue = new QLineEdit ("1.0", rbox);
  hpRadiusValue->setAlignment (Qt::AlignCenter);
  QString rad;
  rad.sprintf ("%0.1f", options->hpRadius);
  hpRadiusValue->setText (rad);
  hpRadiusValue->setToolTip (tr ("Filter radius value"));
  hpRadiusValue->setWhatsThis (hpRadiusValueText);
  rboxLayout->addWidget (hpRadiusValue);
  l40boxLayout->addWidget (rbox);

  QGroupBox *htbox = new QGroupBox (tr ("Height"), this);
  QHBoxLayout *htboxLayout = new QHBoxLayout;
  htbox->setLayout (htboxLayout);
  hpHeightValue = new QLineEdit ("1.0", htbox);
  hpHeightValue->setAlignment (Qt::AlignCenter);
  QString ht;
  ht.sprintf ("%0.2f", options->hpHeight);
  hpHeightValue->setText (ht);
  hpHeightValue->setToolTip (tr ("Filter height value"));
  hpHeightValue->setWhatsThis (hpHeightValueText);
  htboxLayout->addWidget (hpHeightValue);
  l40boxLayout->addWidget (htbox);

  QGroupBox *sbox = new QGroupBox (tr ("Single line neighbors"), this);
  QHBoxLayout *sboxLayout = new QHBoxLayout;
  sbox->setLayout (sboxLayout);
  hpNeighborsValue = new QLineEdit (" 000 ", sbox);
  hpNeighborsValue->setAlignment (Qt::AlignCenter);
  QString nb;
  nb.sprintf ("%d", options->hpNeighbors);
  hpNeighborsValue->setText (nb);
  hpNeighborsValue->setToolTip (tr ("Filter single line neighbors value"));
  hpNeighborsValue->setWhatsThis (hpNeighborsValueText);
  sboxLayout->addWidget (hpNeighborsValue);
  l40boxLayout->addWidget (sbox);


  QGroupBox *mbox = new QGroupBox (tr ("Multi-line neighbors"), this);
  QHBoxLayout *mboxLayout = new QHBoxLayout;
  mbox->setLayout (mboxLayout);
  hpMultiNeighborsValue = new QLineEdit (" 000 ", mbox);
  hpMultiNeighborsValue->setAlignment (Qt::AlignCenter);
  nb.sprintf ("%d", options->hpMultiNeighbors);
  hpMultiNeighborsValue->setText (nb);
  hpMultiNeighborsValue->setToolTip (tr ("Hockey Puck filter multi-line neighbors value"));
  hpMultiNeighborsValue->setWhatsThis (hpMultiNeighborsValueText);
  mboxLayout->addWidget (hpMultiNeighborsValue);
  l40boxLayout->addWidget (mbox);


  vbox->addWidget (l40box);


  QGroupBox *offBox = new QGroupBox (tr ("Local datum offset"), this);
  QHBoxLayout *offBoxLayout = new QHBoxLayout;
  offBox->setLayout (offBoxLayout);


  QGroupBox *datumBox = new QGroupBox (tr ("Datum applied"), this);
  QHBoxLayout *datumBoxLayout = new QHBoxLayout;
  datumBox->setLayout (datumBoxLayout);
  datum = new QCheckBox (datumBox);
  datum->setToolTip (tr ("Check this box if the PFM is already shifted to the local datum"));
  datum->setWhatsThis (datumText);
  connect (datum, SIGNAL (stateChanged (int)), this, SLOT (slotDatumClicked (int)));
  datumBoxLayout->addWidget (datum);
  offBoxLayout->addWidget (datumBox);

  offset = new QDoubleSpinBox (offBox);
  offset->setDecimals (2);
  offset->setRange (-1000.0, 1000.0);
  offset->setSingleStep (10.0);
  offset->setToolTip (tr ("Change the local datum offset value"));
  offset->setWhatsThis (offsetText);
  offset->setValue (0.0);
  offBoxLayout->addWidget (offset);

  egmButton = new QPushButton (tr ("EGM08"));
  egmButton->setToolTip (tr ("Get the ellipsoid to geoid datum offset from EGM08 model"));
  egmButton->setWhatsThis (egmText);
  connect (egmButton, SIGNAL (clicked ()), this, SLOT (slotEgmClicked (void)));
  offBoxLayout->addWidget (egmButton);


  vbox->addWidget (offBox);


  QGroupBox *invBox = new QGroupBox (tr ("Invalidate"), this);
  QHBoxLayout *invBoxLayout = new QHBoxLayout;
  invBox->setLayout (invBoxLayout);


  inv = new QCheckBox (invBox);
  inv->setToolTip (tr ("Check this box if you want to invalidate points instead of marking as suspect"));
  inv->setWhatsThis (invText);
  connect (inv, SIGNAL (stateChanged (int)), this, SLOT (slotInvClicked (int)));
  invBoxLayout->addWidget (inv);


  vbox->addWidget (invBox);


  registerField ("hpRadiusValue", hpRadiusValue);
  registerField ("hpHeightValue", hpHeightValue);
  registerField ("hpNeighborsValue", hpNeighborsValue);
  registerField ("hpMultiNeighborsValue", hpMultiNeighborsValue);
  registerField ("offset", offset, "value", "valueChanged");
}



void 
optionPage::setFileCenter (QString file)
{
  int32_t pfm_handle;
  PFM_OPEN_ARGS open_args;

  strcpy (open_args.list_path, file.toLatin1 ());

  open_args.checkpoint = 0;
  if ((pfm_handle = open_existing_pfm_file (&open_args)) < 0) pfm_error_exit (pfm_error);

  mbr_center.y = open_args.head.mbr.min_y + (open_args.head.mbr.max_y - open_args.head.mbr.min_y) / 2.0;
  mbr_center.x = open_args.head.mbr.min_x + (open_args.head.mbr.max_x - open_args.head.mbr.min_x) / 2.0;

  close_pfm_file (pfm_handle);
}



bool 
optionPage::validatePage ()
{
  if (offset->value () == 0.0 && !datum->isChecked ())
    {
      QMessageBox::critical (this, tr ("pfmFilter datum settings"),
                             tr ("You must check the box that indicates that the PFM is relative to a local datum or set a datum offset to continue!"));

      return (false);
    }

  return (true);
}



void
optionPage::slotDatumClicked (int state)
{
  if (!state)
    {
      options->datum_shifted = NVFalse;
      offset->setEnabled (true);
      egmButton->setEnabled (true);
    }
  else
    {
      options->datum_shifted = NVTrue;
      offset->setEnabled (false);
      egmButton->setEnabled (false);
    }
}



void
optionPage::slotInvClicked (int state)
{
  if (!state)
    {
      options->invalidate = NVFalse;
    }
  else
    {
      options->invalidate = NVTrue;

      QMessageBox::warning (this, tr ("pfmFilter Invalidate points"),
                            tr ("You have chosen to invalidate suspect points instead of just marking them as suspect.<br>"
                                "This is dangerous and shouldn't be done unless you are very sure you know what you are doing!"));
    }
}



//!  Get the EGM value nearest to the center of the displayed area.

void 
optionPage::slotEgmClicked ()
{
  qApp->setOverrideCursor (Qt::WaitCursor);
  qApp->processEvents ();

  float datum_offset = get_egm08 (mbr_center.y, mbr_center.x);
  cleanup_egm08 ();

  qApp->restoreOverrideCursor ();

  if (datum_offset < 999999.0) 
    {
      offset->setValue ((double) datum_offset);
    }
  else
    {
      QMessageBox::warning (this, tr ("pfmFilter Get EGM08 datum offset"), tr ("Error retrieving EGM08 ellipsoid to geoid offset value"));
    }
}



void
optionPage::slotHelp ()
{
  QWhatsThis::enterWhatsThisMode ();
}
