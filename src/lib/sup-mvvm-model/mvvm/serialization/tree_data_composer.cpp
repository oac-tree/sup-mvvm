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

#include "tree_data_composer.h"

#include "standard_tree_data_composer_components.h"

#include <mvvm/model/session_item.h>

#include <stack>

namespace mvvm
{

TreeDataComposer::TreeDataComposer(filter_item_t filter_item)
    : m_filter_item(std::move(filter_item))
{
}

std::unique_ptr<tree_data_t> TreeDataComposer::CreateTreeData(const SessionItem& item) const
{
  std::stack<ItemComposerComponent::component_t> stack;

  stack.push(std::make_unique<ItemComposerComponent>(item, m_filter_item));

  while (!stack.empty())
  {
    auto& top = stack.top();

    if (top->HasNextComponent())
    {
      stack.push(top->TakeNextComponent());
    }
    else
    {
      auto popped = std::move(stack.top());
      stack.pop();
      if (!stack.empty())
      {
        auto& parent = stack.top();
        parent->AddChild(popped->GetTreeData());
      }
      else
      {
        return std::make_unique<tree_data_t>(popped->GetTreeData());
      }
    }
  }

  return {};
}

}  // namespace mvvm
