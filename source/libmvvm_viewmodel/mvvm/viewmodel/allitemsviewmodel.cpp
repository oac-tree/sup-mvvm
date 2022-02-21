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

#include "mvvm/viewmodel/allitemsviewmodel.h"

#include "mvvm/factories/viewmodelcontrollerfactory.h"
#include "mvvm/viewmodel/standardchildrenstrategies.h"
#include "mvvm/viewmodel/standardrowstrategies.h"

namespace mvvm
{
AllItemsViewModel::AllItemsViewModel(ApplicationModel *model, QObject *parent) : ViewModel(parent)
{
  SetController(factory::CreateController<AllChildrenStrategy, LabelDataRowStrategy>(model, this));
}

}  // namespace mvvm
