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

#include "top_items_viewmodel.h"

#include "standard_children_strategies.h"
#include "standard_row_strategies.h"
#include "viewmodel_controller.h"

#include <mvvm/factories/viewmodel_controller_factory.h>

namespace mvvm
{
TopItemsViewModel::TopItemsViewModel(ApplicationModel *model, QObject *parent) : ViewModel(parent)
{
  SetController(factory::CreateController<TopItemsStrategy, LabelDataRowStrategy>(model, this));
}

}  // namespace mvvm
