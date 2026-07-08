/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "custom_metatypes.h"

#include <mvvm/model/item_selection.h>

#include <QMetaType>
#include <mutex>

namespace mvvm
{

namespace
{
// File-scope (not local) static storage duration to comply with MISRA C++ 2023 Rule 6.7.1.
// std::once_flag is constant-initialised, so there is no static-initialisation-order dependency.
std::once_flag register_once_flag;
}  // namespace

void RegisterCustomMetaTypes()
{
  auto register_func = []()
  { (void)qRegisterMetaType<mvvm::ItemSelection>("mvvm::ItemSelection"); };

  std::call_once(register_once_flag, register_func);
}

}  // namespace mvvm
