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

#ifndef MVVM_FACTORIES_ITEM_BACKUP_STRATEGY_FACTORY_H_
#define MVVM_FACTORIES_ITEM_BACKUP_STRATEGY_FACTORY_H_

#include <mvvm/model/i_item_backup_strategy.h>

#include <memory>

namespace mvvm
{
class IItemFactory;

//! Returns default strategy for item backup based on a TreeData objects.
MVVM_MODEL_EXPORT std::unique_ptr<IItemBackupStrategy> CreateItemTreeDataBackupStrategy(
    const IItemFactory* factory);

}  // namespace mvvm

#endif  // MVVM_FACTORIES_ITEM_BACKUP_STRATEGY_FACTORY_H_
