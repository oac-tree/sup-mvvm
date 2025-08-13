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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "item_backup_strategy_factory.h"

#include "standard_item_backup_strategies.h"

#include <mvvm/model/session_item.h>

namespace mvvm
{

// cppcheck-suppress unusedFunction
std::unique_ptr<IItemBackupStrategy> CreateItemTreeDataBackupStrategy(const IItemFactory* factory)
{
  return std::make_unique<TreeDataItemBackupStrategy>(factory);
}

// cppcheck-suppress unusedFunction
std::unique_ptr<IItemBackupStrategy> CreateItemCloneBackupStrategy()
{
  return std::make_unique<ItemCloneBackupStrategy>();
}

}  // namespace mvvm
