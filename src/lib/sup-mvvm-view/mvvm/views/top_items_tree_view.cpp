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

#include "top_items_tree_view.h"

#include <mvvm/viewmodel/top_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace mvvm
{
TopItemsTreeView::TopItemsTreeView(ISessionModel *model, QWidget *parent_widget)
    : AbstractItemView(parent_widget), m_tree_view(new QTreeView)
{
  SetComponentProvider(CreateProvider().ViewModel<TopItemsViewModel>(model).View(m_tree_view));
}

QTreeView *TopItemsTreeView::GetTreeView() const
{
  return m_tree_view;
}

void TopItemsTreeView::UpdateView()
{
  m_tree_view->expandAll();
}

}  // namespace mvvm
