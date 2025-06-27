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

#ifndef MVVM_FACTORIES_STANDARD_ITEM_BACKUP_STRATEGIES_H
#define MVVM_FACTORIES_STANDARD_ITEM_BACKUP_STRATEGIES_H

#include <mvvm/model/i_item_backup_strategy.h>
#include <mvvm/serialization/tree_data_fwd.h>

namespace mvvm
{

class IItemFactory;
class ITreeDataItemConverter;

/**
 * @brief The TreeDataItemBackupStrategy class saves/restores SessionItem using TreeData objects.
 */
class TreeDataItemBackupStrategy : public mvvm::IItemBackupStrategy
{
public:
  explicit TreeDataItemBackupStrategy(const IItemFactory* factory);
  ~TreeDataItemBackupStrategy() override;

  TreeDataItemBackupStrategy(const TreeDataItemBackupStrategy& other) = delete;
  TreeDataItemBackupStrategy& operator=(const TreeDataItemBackupStrategy& other) & = delete;
  TreeDataItemBackupStrategy(TreeDataItemBackupStrategy&&) = delete;
  TreeDataItemBackupStrategy& operator=(TreeDataItemBackupStrategy&&) = delete;

  void SaveItem(const mvvm::SessionItem& item) override;

  std::unique_ptr<mvvm::SessionItem> RestoreItem() const override;

private:
  std::unique_ptr<ITreeDataItemConverter> m_converter;
  std::unique_ptr<tree_data_t> m_tree_data;
};

/**
 * @brief The ItemCloneBackupStrategy class saves/restores SessionItem using TreeData objects.
 */
class ItemCloneBackupStrategy : public mvvm::IItemBackupStrategy
{
public:
  ItemCloneBackupStrategy() = default;
  ~ItemCloneBackupStrategy() override;

  ItemCloneBackupStrategy(const ItemCloneBackupStrategy& other) = delete;
  ItemCloneBackupStrategy& operator=(const ItemCloneBackupStrategy& other) & = delete;
  ItemCloneBackupStrategy(ItemCloneBackupStrategy&&) = delete;
  ItemCloneBackupStrategy& operator=(ItemCloneBackupStrategy&&) = delete;

  void SaveItem(const mvvm::SessionItem& item) override;

  std::unique_ptr<mvvm::SessionItem> RestoreItem() const override;

private:
  std::unique_ptr<mvvm::SessionItem> m_backup;
};


}  // namespace mvvm

#endif  // MVVM_FACTORIES_STANDARD_ITEM_BACKUP_STRATEGIES_H
