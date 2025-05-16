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

#include "model_test_helper.h"

#include <mvvm/model/compound_item.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>

#include <stack>

namespace mvvm::test
{

namespace
{

/**
 * @brief Returns info for given item.
 */
ItemInfo GetItemInfo(SessionItem* item)
{
  return {item, item->GetIdentifier(), utils::PathFromItem(item)};
}
}  // namespace

std::unique_ptr<ItemTreeData> CreateTestData(std::size_t max_depth_level,
                                             std::size_t children_per_node)
{
  if (max_depth_level < 1)
  {
    return {};
  }

  auto result = std::make_unique<ItemTreeData>();
  result->model = std::make_unique<SessionModel>();
  auto model = result->model.get();

  struct Node
  {
    SessionItem* item{nullptr};
    std::size_t level{0U};
  };

  std::stack<Node> stack;
  stack.push({model->GetRootItem(), 1U});
  result->item_info.push_back((GetItemInfo(model->GetRootItem())));

  while (!stack.empty())
  {
    auto [parent, level] = stack.top();
    stack.pop();
    if (level < max_depth_level)
    {
      for (size_t i_child = 0; i_child < children_per_node; ++i_child)
      {
        auto child = model->InsertItem<CompoundItem>(parent);
        child->RegisterTag(mvvm::TagInfo::CreateUniversalTag("tag"),
                           /*set_default*/ true);
        result->item_info.push_back((GetItemInfo(child)));
      }

      // populating stack in reverse order to match linear order of children in
      // std::vector<ItemInfo>
      auto children = parent->GetAllItems();
      for (auto iter = children.rbegin(); iter != children.rend(); ++iter)
      {
        stack.push({*iter, level + 1});
      }
    }
  }

  return result;
}

}  // namespace mvvm::test
