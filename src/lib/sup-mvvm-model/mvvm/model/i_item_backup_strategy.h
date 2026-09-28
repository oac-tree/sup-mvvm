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

#ifndef MVVM_MODEL_I_ITEM_BACKUP_STRATEGY_H_
#define MVVM_MODEL_I_ITEM_BACKUP_STRATEGY_H_

#include <memory>

namespace mvvm
{

class SessionItem;

/**
 * @brief The IItemBackupStrategy is an interface to backup items for later restore.
 */
class IItemBackupStrategy
{
public:
  virtual ~IItemBackupStrategy() noexcept = default;

  /**
   * @brief Save item content.
   */
  virtual void SaveItem(const SessionItem& item) = 0;

  /**
   * @brief Restore item from saved content.
   */
  virtual std::unique_ptr<SessionItem> RestoreItem() const = 0;

protected:
  IItemBackupStrategy() = default;
  IItemBackupStrategy(const IItemBackupStrategy&) = default;
  IItemBackupStrategy(IItemBackupStrategy&&) noexcept = default;
  IItemBackupStrategy& operator=(const IItemBackupStrategy&) & noexcept = default;
  IItemBackupStrategy& operator=(IItemBackupStrategy&&) & noexcept = default;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_I_ITEM_BACKUP_STRATEGY_H_
