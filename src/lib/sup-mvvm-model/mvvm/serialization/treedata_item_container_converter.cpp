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

#include "treedata_item_container_converter.h"

#include "tree_data.h"
#include "treedata_taginfo_converter.h"

#include <mvvm/model/session_item.h>
#include <mvvm/model/session_item_container.h>
#include <mvvm/model/taginfo.h>

namespace
{
const std::string kItemContainerElementType = "ItemContainer";
}  // namespace

namespace mvvm::ContainerConverter
{
bool IsItemContainerConvertible(const tree_data_t &tree_data)
{
  const bool correct_type = tree_data.GetNodeName() == kItemContainerElementType;
  const bool correct_attributes = tree_data.GetNumberOfAttributes() == 0;
  const bool correct_children = tree_data.GetNumberOfChildren() > 0;  // at least TagInfo
  return correct_type && correct_attributes && correct_children;
}

std::unique_ptr<SessionItemContainer> ToSessionItemContainer(const tree_data_t &tree_data,
                                                             const create_item_t &func)
{
  if (!IsItemContainerConvertible(tree_data))
  {
    throw RuntimeException("Error in SessionItemContainerConverter: invalid TreeData");
  }

  auto tag_info = ToTagInfo(tree_data.Children().at(0));

  auto result = std::make_unique<SessionItemContainer>(tag_info);
  PopulateSessionItemContainer(tree_data, *result, func);
  return result;
}

void PopulateSessionItemContainer(const tree_data_t &tree_data, SessionItemContainer &container,
                                  const create_item_t &func)
{
  if (!IsItemContainerConvertible(tree_data))
  {
    throw RuntimeException("Error in SessionItemContainerConverter: invalid TreeData");
  }

  if (!container.IsEmpty())
  {
    throw RuntimeException("Error in PopulateSessionItemContainer: container is not empty");
  }

  // first child in TreeData corresponds to TagInfo which has been already processed
  for (auto it = std::next(tree_data.Children().begin()); it != tree_data.Children().end(); ++it)
  {
    // creating item with the function provided and inserting in the container
    container.InsertItem(func(*it), container.GetItemCount());
  }
}

std::unique_ptr<tree_data_t> ToTreeData(const SessionItemContainer &container,
                                     const create_treedata_t &func)
{
  auto result = std::make_unique<tree_data_t>(kItemContainerElementType);
  result->AddChild(ToTreeData(container.GetTagInfo()));
  for (const auto &item : container)
  {
    result->AddChild(*func(*item));
  }
  return result;
}

}  // namespace mvvm::ContainerConverter
