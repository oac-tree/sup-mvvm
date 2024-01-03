/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "treedata_itemdata_converter.h"

#include "treedata.h"
#include "treedata_variant_converter.h"

#include <mvvm/model/sessionitem_data.h>

#include <stdexcept>

namespace
{
const std::string kItemDataElementType = "ItemData";
}

namespace mvvm
{
TreeDataItemDataConverter::TreeDataItemDataConverter() = default;

std::unique_ptr<TreeData> TreeDataItemDataConverter::ToTreeData(
    const SessionItemData &item_data) const
{
  auto result = std::make_unique<TreeData>(kItemDataElementType);
  for (const auto &datarole : item_data)
  {
    result->AddChild(::mvvm::ToTreeData(datarole));
  }
  return result;
}

std::unique_ptr<SessionItemData> TreeDataItemDataConverter::ToSessionItemData(
    const TreeData &tree_data) const
{
  auto result = std::make_unique<SessionItemData>();
  PopulateItemData(tree_data, *result);
  return result;
}

void TreeDataItemDataConverter::PopulateItemData(const TreeData &tree_data,
                                                 SessionItemData &item_data) const
{
  if (!IsSessionItemDataConvertible(tree_data))
  {
    throw std::runtime_error("Given TreeData can't be converted in SessionItemData object");
  }

  // In the future filtering of roles will be implemented
  for (const auto &child : tree_data.Children())
  {
    auto datarole = ToDataRole(child);
    item_data.SetData(datarole.first, datarole.second);
  }
}

bool TreeDataItemDataConverter::IsSessionItemDataConvertible(const TreeData &tree_data) const
{
  return tree_data.GetType() == kItemDataElementType && tree_data.GetNumberOfAttributes() == 0;
  // there is no sence to require empty content, it might still contains '\n' symbol,
  // depending on the way TreeData was constructed from XML content.
}

}  // namespace mvvm
