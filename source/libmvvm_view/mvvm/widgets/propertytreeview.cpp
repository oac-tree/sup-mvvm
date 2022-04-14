/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/widgets/propertytreeview.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/propertyviewmodel.h"
#include "mvvm/viewmodel/viewmodel.h"

#include <QTreeView>

namespace mvvm
{
PropertyTreeView::PropertyTreeView(QWidget* parent)
    : AbstractItemView(CreateViewModel<PropertyViewModel>, new QTreeView, nullptr, parent)
{
  GetTreeView()->setHeaderHidden(false);
  GetTreeView()->setRootIsDecorated(false);
  GetTreeView()->setEditTriggers(QAbstractItemView::AllEditTriggers);  // provide one click editing
  GetTreeView()->setAlternatingRowColors(true);
}

void PropertyTreeView::SetItem(SessionItem* item)
{
  AbstractItemView::SetItem(item);
  GetTreeView()->expandAll();
}

QTreeView* PropertyTreeView::GetTreeView()
{
  return dynamic_cast<QTreeView*>(GetView());
}

}  // namespace mvvm
