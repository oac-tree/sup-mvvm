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

#ifndef MVVM_MODEL_FUNCTION_TYPES_H
#define MVVM_MODEL_FUNCTION_TYPES_H

#include "mvvm/core/variant.h"

#include <functional>
#include <memory>

namespace mvvm
{
class SessionItem;

//! Definition for item factory funciton.
using item_factory_func_t = std::function<std::unique_ptr<SessionItem>()>;

//! Creates factory function for item of specific type.
template <typename T>
auto ItemFactoryFunction()
{
  return []() { return std::make_unique<T>(); };
}

//! Function to set the data to SessionItem. Used in PresentationItem to set data through the
//! composer.
using item_setdata_function_t = std::function<bool(SessionItem*, const variant_t& value, int role)>;

}  // namespace ModelView

#endif  // MVVM_MODEL_FUNCTION_TYPES_H
