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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "property_tree_view.h"

#include <mvvm/model/session_item.h>
#include <mvvm/providers/item_view_component_provider.h>
#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <QTreeView>
#include <QVBoxLayout>

namespace mvvm
{
PropertyTreeView::PropertyTreeView(QWidget* parent_widget)
    : AbstractItemView(parent_widget), m_tree_view(new QTreeView)
{
  SetComponentProvider(CreateProvider<PropertyViewModel>(m_tree_view));
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
}

}  // namespace mvvm
