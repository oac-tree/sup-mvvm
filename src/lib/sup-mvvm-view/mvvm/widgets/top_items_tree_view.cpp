/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "top_items_tree_view.h"

#include "item_view_component_provider.h"

#include <mvvm/viewmodel/top_items_viewmodel.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace mvvm
{
TopItemsTreeView::TopItemsTreeView(SessionModelInterface *model, QWidget *parent)
    : AbstractItemView(parent), m_tree_view(new QTreeView)
{
  SetComponentProvider(CreateProvider<TopItemsViewModel>(m_tree_view, model));
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
