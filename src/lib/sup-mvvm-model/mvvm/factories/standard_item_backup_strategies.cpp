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

#include "standard_item_backup_strategies.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/serialization/tree_data.h>
#include <mvvm/serialization/tree_data_item_converter.h>

namespace mvvm
{

TreeDataItemBackupStrategy::TreeDataItemBackupStrategy(const IItemFactory* factory)
    : m_converter(
          std::make_unique<mvvm::TreeDataItemConverter>(factory, mvvm::ConverterMode::kClone))
{
}

TreeDataItemBackupStrategy::~TreeDataItemBackupStrategy() = default;

void TreeDataItemBackupStrategy::SaveItem(const SessionItem& item)
{
  m_tree_data = m_converter->ToTreeData(item);
}

std::unique_ptr<SessionItem> TreeDataItemBackupStrategy::RestoreItem() const
{
  if (!m_tree_data)
  {
    throw mvvm::InvalidOperationException("Absent backup");
  }
  return m_converter->ToSessionItem(*m_tree_data);
}

// ------------------------------------------------------------------------------------------------
// ItemCloneBackupStrategy
// ------------------------------------------------------------------------------------------------

ItemCloneBackupStrategy::~ItemCloneBackupStrategy() = default;

void ItemCloneBackupStrategy::SaveItem(const SessionItem& item)
{
  m_backup = item.Clone();
}

std::unique_ptr<SessionItem> ItemCloneBackupStrategy::RestoreItem() const
{
  if (!m_backup)
  {
    throw mvvm::InvalidOperationException("Absent backup");
  }
  return m_backup->Clone();
}

}  // namespace mvvm
