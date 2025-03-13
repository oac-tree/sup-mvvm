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

#include "item_backup_strategy_factory.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/serialization/tree_data.h>
#include <mvvm/serialization/tree_data_item_converter.h>

namespace
{

//! Implements copy strategy for SessionItem using TreeDataItemConverter.
class TreeDataItemBackupStrategy : public mvvm::IItemBackupStrategy
{
public:
  explicit TreeDataItemBackupStrategy(const ::mvvm::IItemFactory* factory)
      : m_converter(
            std::make_unique<mvvm::TreeDataItemConverter>(factory, mvvm::ConverterMode::kClone))
  {
  }

  void SaveItem(const mvvm::SessionItem& item) override
  {
    m_tree_data = m_converter->ToTreeData(item);
  }

  std::unique_ptr<mvvm::SessionItem> RestoreItem() const override
  {
    if (!m_tree_data)
    {
      throw mvvm::InvalidOperationException("Absent backup");
    }
    return m_converter->ToSessionItem(*m_tree_data);
  }

private:
  std::unique_ptr<mvvm::ITreeDataItemConverter> m_converter;
  std::unique_ptr<mvvm::tree_data_t> m_tree_data;
};

}  // namespace

namespace mvvm
{

std::unique_ptr<IItemBackupStrategy> CreateItemTreeDataBackupStrategy(const IItemFactory* factory)
{
  return std::make_unique<TreeDataItemBackupStrategy>(factory);
}

}  // namespace mvvm
