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

#include "session_item_composer.h"

#include "session_item_composer_components.h"
#include "tree_data.h"
#include "tree_data_constants.h"

#include <mvvm/model/i_item_factory.h>
#include <mvvm/model/session_item.h>

#include <stack>

namespace mvvm
{

SessionItemComposer::SessionItemComposer(const ItemComposerContext& context) : m_context(context) {}

std::unique_ptr<SessionItem> SessionItemComposer::CreateSessionItem(
    const tree_data_t& tree_data) const
{
  auto model_type = tree_data.GetAttribute(constants::kTypelAttributeKey);
  auto result = m_context.factory->CreateItem(model_type);

  std::stack<ItemComposerComponent::component_t> stack;

  stack.push(std::make_unique<SessionItemFromTreeData>(m_context, tree_data, *result));

  while (!stack.empty())
  {
    auto& top = stack.top();

    if (top->HasNextComponent())
    {
      stack.push(top->TakeNextComponent());
    }
    else
    {
      stack.pop();
    }
  }

  return result;
}

}  // namespace mvvm
