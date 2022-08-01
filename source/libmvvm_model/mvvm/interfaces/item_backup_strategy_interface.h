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

#ifndef MVVM_INTERFACES_ITEM_BACKUP_STRATEGY_INTERFACE_H_
#define MVVM_INTERFACES_ITEM_BACKUP_STRATEGY_INTERFACE_H_

#include <mvvm/model_export.h>

#include <memory>

namespace mvvm
{

class SessionItem;

//! Interface to backup items for later restore.

class MVVM_MODEL_EXPORT ItemBackupStrategyInterface
{
public:
  virtual ~ItemBackupStrategyInterface() = default;

  //! Save item's content.
  virtual void SaveItem(const SessionItem& item) = 0;

  //! Restore item from saved content.
  virtual std::unique_ptr<SessionItem> RestoreItem() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_ITEM_BACKUP_STRATEGY_INTERFACE_H_
