
/****************************************************************************
**
** Copyright (C) 2007-2009 Kevin Clague. All rights reserved.
** Copyright (C) 2015 - 2018 Trevor SANDY. All rights reserved.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "globals.h"
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>

#include "meta.h"
#include "metagui.h"
#include "metaitem.h"

/*****************************************************************
 *
 * Global to pli
 *
 ****************************************************************/

class GlobalPliPrivate
{
public:
  Meta      meta;
  QString    topLevelFile;
  QList<MetaGui *> children;
  MetaGui *viewAngle;
  MetaGui *scale;
  bool     bom;

  GlobalPliPrivate(QString &_topLevelFile, Meta &_meta, bool _bom = false)
  {
    topLevelFile = _topLevelFile;
    meta = _meta;
    bom = _bom;
    MetaItem mi; // examine all the globals and then return

    mi.sortedGlobalWhere(meta,topLevelFile,"ZZZZZZZ");
  }
};

GlobalPliDialog::GlobalPliDialog(
  QString &topLevelFile, Meta &meta, bool bom)
{
  data = new GlobalPliPrivate(topLevelFile,meta,bom);

  if (bom) {
    setWindowTitle(tr("Bill of Materials Setup"));
  } else {
    setWindowTitle(tr("Parts List Setup"));
  }

  QTabWidget  *tab = new QTabWidget(NULL);
  QVBoxLayout *layout = new QVBoxLayout(NULL);
  QVBoxLayout *childlayout = new QVBoxLayout(NULL);
  setLayout(layout);
  layout->addWidget(tab);

  QWidget *widget;
  QGridLayout *grid;

  MetaGui *child;
  QGroupBox *box;

  PliMeta *pliMeta = bom ? &data->meta.LPub.bom : &data->meta.LPub.pli;

  /*
   * Background/Border tab
   */

  widget = new QWidget(NULL);
  grid = new QGridLayout(NULL);
  widget->setLayout(grid);

  if ( ! bom) {
    box = new QGroupBox("Parts List");
    grid->addWidget(box);
    child = new CheckBoxGui("Show Parts List",&pliMeta->show, box);
    data->children.append(child);
  }

  box = new QGroupBox("Background");
  grid->addWidget(box);
  child = new BackgroundGui(&pliMeta->background,box);
  data->children.append(child);

  box = new QGroupBox("Border");
  grid->addWidget(box);
  child = new BorderGui(&pliMeta->border,box);
  data->children.append(child);
  
  box = new QGroupBox("Margins");
  grid->addWidget(box);
  child = new UnitsGui("",&pliMeta->margin,box);
  data->children.append(child);
  
  box = new QGroupBox("Constrain");
  grid->addWidget(box);
  child = new ConstrainGui("",&pliMeta->constrain,box);
  data->children.append(child);

  tab->addTab(widget,"Background/Border");

  /*
   * Contents tab
   */

  widget = new QWidget(NULL);
  grid = new QGridLayout(NULL);
  widget->setLayout(grid);

  box = new QGroupBox("Part Images");
  grid->addWidget(box);
  box->setLayout(childlayout);

  child = new DoubleSpinGui("Scale",
    &pliMeta->modelScale,
    pliMeta->modelScale._min,
    pliMeta->modelScale._max,
    0.01);
  data->children.append(child);
  data->scale = child;
  childlayout->addWidget(child);

  child = new UnitsGui("Margins",&pliMeta->part.margin);
  data->children.append(child);
  childlayout->addWidget(child);

  box = new QGroupBox("Part Orientation");
  grid->addWidget(box);
  child = new FloatsGui("Lattitude","Longitude",&pliMeta->angle,box);
  data->viewAngle = child;
  data->children.append(child);

  if ( ! bom) {
    box = new QGroupBox("Submodels");
    grid->addWidget(box);
    child = new CheckBoxGui("Show in Parts List",&pliMeta->includeSubs,box);
    data->children.append(child);
  }

  box = new QGroupBox("Part Counts");
  grid->addWidget(box);
  child = new NumberGui(&pliMeta->instance,box);
  data->children.append(child);

  box = new QGroupBox("Part Annotation");
  grid->addWidget(box);
  child = new NumberGui(&pliMeta->annotate,box);
  data->children.append(child);

  tab->addTab(widget,"Contents");

  /*
   * PLI Sort
   */

  widget = new QWidget(NULL);
  QVBoxLayout *vlayout = new QVBoxLayout(NULL);
  widget->setLayout(vlayout);

  box = new QGroupBox("Annotation Options");
  vlayout->addWidget(box);
  child = new PliAnnotationGui("",&pliMeta->annotation,box);
  data->children.append(child);

  box = new QGroupBox("Sort Options");
  vlayout->addWidget(box);
  child = new PliSortGui("",&pliMeta->sortBy,box);
  data->children.append(child);

  //spacer
  QSpacerItem *vSpacer = new QSpacerItem(1,1,QSizePolicy::Fixed,QSizePolicy::Expanding);
  vlayout->addSpacerItem(vSpacer);

  tab->addTab(widget,"Annotations/Sorting");

  /*
   * Sub-Model colors
   */

  widget = new QWidget();
  vlayout = new QVBoxLayout(NULL);
  widget->setLayout(vlayout);

  box = new QGroupBox(tr("Sub-Model Level Colors"));
  vlayout->addWidget(box);
  child = new SubModelColorGui(&pliMeta->subModelColor,box);
  data->children.append(child);

  //spacer
  vlayout->addSpacerItem(vSpacer);

  tab->addTab(widget,"SubModel Colors");

  QDialogButtonBox *buttonBox;

  buttonBox = new QDialogButtonBox(NULL);
  buttonBox->addButton(QDialogButtonBox::Ok);
  connect(buttonBox,SIGNAL(accepted()),SLOT(accept()));
  buttonBox->addButton(QDialogButtonBox::Cancel);
  connect(buttonBox,SIGNAL(rejected()),SLOT(cancel()));

  layout->addWidget(buttonBox);

  setModal(true);
  setMinimumSize(40,20);
}

void GlobalPliDialog::getPliGlobals(
  QString topLevelFile, Meta &meta)
{
  GlobalPliDialog *dialog = new GlobalPliDialog(topLevelFile, meta, false);
  dialog->exec();
}

void GlobalPliDialog::getBomGlobals(
  QString topLevelFile, Meta &meta)
{
  GlobalPliDialog *dialog = new GlobalPliDialog(topLevelFile, meta, true);
  dialog->exec();
}

void GlobalPliDialog::accept()
{
  if (data->scale->modified ||
      data->viewAngle->modified) {
    clearPliCache();
  }

  MetaItem mi;

  mi.beginMacro("Global Pli");

  MetaGui *child;

  foreach(child,data->children) {
    child->apply(data->topLevelFile);
  }
  mi.endMacro();
  QDialog::accept();
}

void GlobalPliDialog::cancel()
{
  QDialog::reject();
}
