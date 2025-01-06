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

#include "all_items_viewmodel.h"

#include "standard_children_strategies.h"
#include "standard_row_strategies.h"
#include "viewmodel_controller_factory.h"

namespace mvvm
{

AllItemsViewModel::AllItemsViewModel(ISessionModel *model, QObject *parent, bool show_hidden)
    : ViewModel(parent)
{
  if (show_hidden)
  {
    SetController(
        factory::CreateController<AllChildrenStrategy, LabelDataRowStrategy>(model, this));
  }
  else
  {
    SetController(
        factory::CreateController<AllVisibleChildrenStrategy, LabelDataRowStrategy>(model, this));
  }
}

}  // namespace mvvm
