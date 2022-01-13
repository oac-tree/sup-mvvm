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

#include "mvvm/viewmodel/viewitemfactory.h"

#include "mvvm/viewmodel/presentationitemfactory.h"
#include "mvvm/viewmodelbase/viewitem.h"

namespace mvvm
{
std::unique_ptr<ViewItem> CreateDisplayNameViewItem(SessionItem* item)
{
  return std::make_unique<ViewItem>(CreateDisplayNamePresentation(item));
}

std::unique_ptr<ViewItem> CreateDataViewItem(SessionItem* item, item_setdata_function_t set_func)
{
  return std::make_unique<ViewItem>(CreateDataPresentation(item, set_func));
}

}  // namespace ModelView
