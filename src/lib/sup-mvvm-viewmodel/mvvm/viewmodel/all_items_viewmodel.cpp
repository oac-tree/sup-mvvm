/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "all_items_viewmodel.h"

#include "standard_children_strategies.h"
#include "standard_row_strategies.h"

#include <mvvm/factories/viewmodel_controller_factory.h>

namespace mvvm
{
AllItemsViewModel::AllItemsViewModel(SessionModelInterface *model, QObject *parent)
    : ViewModel(parent)
{
  SetController(factory::CreateController<AllChildrenStrategy, LabelDataRowStrategy>(model, this));
}

int AllItemsViewModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 2;
}

}  // namespace mvvm
