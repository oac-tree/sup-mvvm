/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "property_tree_view.h"

#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>

#include <QAbstractItemModel>
#include <QTreeView>

namespace mvvm
{

PropertyTreeView::PropertyTreeView(QWidget* parent_widget)
    : AbstractItemView(parent_widget), m_tree_view(new QTreeView)
{
  SetComponentProvider(CreateProvider<PropertyViewModel>(m_tree_view));

  // Keep the tree fully expanded at all times, so that nested properties (e.g. a position vector,
  // or the parameters of a group's current item) are always visible, including rows added
  // dynamically when a group's selection changes.
  connect(m_tree_view->model(), &QAbstractItemModel::rowsInserted, m_tree_view,
          [this]() { m_tree_view->expandAll(); });
}

QTreeView* PropertyTreeView::GetTreeView() const
{
  return m_tree_view;
}

void PropertyTreeView::UpdateView()
{
  m_tree_view->setHeaderHidden(false);
  m_tree_view->setRootIsDecorated(false);
  m_tree_view->setEditTriggers(QAbstractItemView::AllEditTriggers);  // provide one click editing
  m_tree_view->setAlternatingRowColors(true);
  m_tree_view->expandAll();
}

}  // namespace mvvm
