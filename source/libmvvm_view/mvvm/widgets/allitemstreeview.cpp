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

#include "mvvm/widgets/allitemstreeview.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/viewmodel/allitemsviewmodel.h"

#include <QTreeView>

namespace mvvm
{
AllItemsTreeView::AllItemsTreeView(ApplicationModel* model, QWidget* parent)
    : AbstractItemView(CreateViewModel<AllItemsViewModel>, new QTreeView, model, parent)
{
  GetTreeView()->expandAll();
  GetTreeView()->resizeColumnToContents(0);
}

QTreeView* AllItemsTreeView::GetTreeView()
{
  return dynamic_cast<QTreeView*>(GetView());
}

}  // namespace mvvm
