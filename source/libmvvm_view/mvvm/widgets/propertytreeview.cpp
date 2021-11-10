/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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

#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/applicationmodel.h"
#include "mvvm/viewmodel/propertyviewmodel.h"
#include "mvvm/viewmodel/viewmodel.h"

#include <QTreeView>

namespace ModelView
{
PropertyTreeView::PropertyTreeView(QWidget* parent) : ItemsTreeView(parent)
{
  GetTreeView()->setHeaderHidden(false);
  // provide one click editing
  GetTreeView()->setEditTriggers(QAbstractItemView::AllEditTriggers);
  GetTreeView()->setAlternatingRowColors(true);
}

void PropertyTreeView::SetRootSessionItem(SessionItem *item)
{
  SetItem(item);
}

void PropertyTreeView::SetItem(SessionItem* item)
{
  if (!item)
  {
    Reset();
    return;
  }

  auto application_model = dynamic_cast<ApplicationModel*>(item->GetModel());
  if (!application_model)
  {
    throw std::runtime_error("Error in PropertyTreeView: wrong type model");
  }

  SetViewModel(std::make_unique<PropertyViewModel>(application_model));

  GetViewModel()->SetRootSessionItem(item);
  GetTreeView()->setRootIsDecorated(false);
  GetTreeView()->expandAll();
}

PropertyTreeView::~PropertyTreeView() = default;

}  // namespace ModelView
