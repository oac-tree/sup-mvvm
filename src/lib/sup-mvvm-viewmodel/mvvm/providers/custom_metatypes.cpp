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

void RegisterCustomMetaTypes()
{
  static std::once_flag register_once_flag;
  auto register_func = []()
  { (void)qRegisterMetaType<mvvm::ItemSelection>("mvvm::ItemSelection"); };

  std::call_once(register_once_flag, register_func);
}

}  // namespace mvvm
