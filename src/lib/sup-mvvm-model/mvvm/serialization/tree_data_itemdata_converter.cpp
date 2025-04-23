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

#include "tree_data_itemdata_converter.h"

#include "tree_data.h"
#include "tree_data_variant_converter.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item_data.h>

namespace
{
const std::string kItemDataElementType = "ItemData";
}

namespace mvvm
{
TreeDataItemDataConverter::TreeDataItemDataConverter() = default;

std::unique_ptr<tree_data_t> TreeDataItemDataConverter::ToTreeData(
    const SessionItemData &item_data) const
{
  auto result = std::make_unique<tree_data_t>(kItemDataElementType);
  for (const auto &role_data : item_data)
  {
    result->AddChild(::mvvm::ToTreeData(role_data));
  }
  return result;
}

std::unique_ptr<SessionItemData> TreeDataItemDataConverter::ToSessionItemData(
    const tree_data_t &tree_data) const
{
  auto result = std::make_unique<SessionItemData>();
  PopulateItemData(tree_data, *result);
  return result;
}

void TreeDataItemDataConverter::PopulateItemData(const tree_data_t &tree_data,
                                                 SessionItemData &item_data) const
{
  if (!IsSessionItemDataConvertible(tree_data))
  {
    throw RuntimeException(
        "Error in TreeDataItemDataConverter: given TreeData can't be converted in SessionItemData "
        "object");
  }

  // In the future filtering of roles will be implemented
  for (const auto &child : tree_data.Children())
  {
    auto role_data = ToRoleData(child);
    item_data.SetData(role_data.second, role_data.first);
  }
}

bool TreeDataItemDataConverter::IsSessionItemDataConvertible(const tree_data_t &tree_data) const
{
  return tree_data.GetNodeName() == kItemDataElementType && tree_data.GetNumberOfAttributes() == 0;
  // there is no sence to require empty content, it might still contains '\n' symbol,
  // depending on the way TreeData was constructed from XML content.
}

}  // namespace mvvm
