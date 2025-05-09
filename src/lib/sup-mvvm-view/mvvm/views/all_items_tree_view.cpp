/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "all_items_tree_view.h"

#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/views/item_view_component_provider.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace mvvm
{

AllItemsTreeView::AllItemsTreeView(ISessionModel* model, QWidget* parent_widget, bool show_hidden)
    : AbstractItemView(parent_widget), m_tree_view(new QTreeView)
{
  auto viewmodel = std::make_unique<AllItemsViewModel>(model, parent_widget, show_hidden);
  SetComponentProvider(
      std::make_unique<ItemViewComponentProvider>(std::move(viewmodel), m_tree_view));
}

QTreeView* AllItemsTreeView::GetTreeView() const
{
  return m_tree_view;
}

void AllItemsTreeView::UpdateView()
{
  m_tree_view->expandAll();
}

}  // namespace mvvm
