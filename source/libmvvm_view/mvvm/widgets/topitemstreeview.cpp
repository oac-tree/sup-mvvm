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

#include "mvvm/widgets/topitemstreeview.h"

#include "mvvm/viewmodel/applicationmodel.h"
#include "mvvm/viewmodel/topitemsviewmodel.h"

namespace ModelView
{
TopItemsTreeView::TopItemsTreeView(ApplicationModel* model, QWidget* parent) : ItemsTreeView(parent)
{
  SetApplicationModel(model);
}

void TopItemsTreeView::SetApplicationModel(ApplicationModel* model)
{
  if (!model)
  {
    return;
  }

  SetViewModel(std::make_unique<TopItemsViewModel>(model));
}

TopItemsTreeView::~TopItemsTreeView() = default;

}  // namespace ModelView
