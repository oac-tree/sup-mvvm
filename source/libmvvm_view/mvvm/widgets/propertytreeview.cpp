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
#include "mvvm/widgets/itemviewcomponentprovider.h"

#include <QTreeView>
#include <QVBoxLayout>

namespace mvvm
{
PropertyTreeView::PropertyTreeView(QWidget* parent)
    : AbstractItemView(parent), m_tree_view(new QTreeView)
{
  auto provider =
      std::make_unique<ItemViewComponentProvider>(CreateViewModel<PropertyViewModel>, m_tree_view);
  SetComponentProvider(std::move(provider));
}

void PropertyTreeView::UpdateView()
{
  m_tree_view->setHeaderHidden(false);
  m_tree_view->setRootIsDecorated(false);
  m_tree_view->setEditTriggers(QAbstractItemView::AllEditTriggers);  // provide one click editing
  m_tree_view->setAlternatingRowColors(true);
}

}  // namespace mvvm
