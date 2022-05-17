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
#include "mvvm/widgets/itemviewcomponentprovider.h"

#include <QTreeView>
#include <QVBoxLayout>

namespace mvvm
{
AllItemsTreeView::AllItemsTreeView(ApplicationModel* model, QWidget* parent)
    : AbstractItemView(parent), m_tree_view(new QTreeView)
{
  auto provider = std::make_unique<ItemViewComponentProvider>(CreateViewModel<AllItemsViewModel>,
                                                              m_tree_view);
  provider->SetApplicationModel(model);
  SetComponentProvider(std::move(provider));

  m_tree_view->expandAll();
  m_tree_view->resizeColumnToContents(0);
  layout()->addWidget(m_tree_view);
}

QTreeView* AllItemsTreeView::GetTreeView()
{
  return m_tree_view;
}

}  // namespace mvvm
