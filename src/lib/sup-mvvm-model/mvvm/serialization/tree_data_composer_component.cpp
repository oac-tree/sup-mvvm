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

#include "tree_data_composer_component.h"

namespace mvvm
{

TreeDataComposerComponent::TreeDataComposerComponent(const std::string& node_name)
    : m_tree_data(node_name)
{
}

TreeDataComposerComponent::~TreeDataComposerComponent() = default;

tree_data_t TreeDataComposerComponent::MoveTreeData()
{
  return std::move(m_tree_data);
}

tree_data_t TreeDataComposerComponent::GetTreeData() const
{
  return m_tree_data;
}

void TreeDataComposerComponent::AddChild(const tree_data_t& tree_data)
{
  m_tree_data.AddChild(tree_data);
}

void TreeDataComposerComponent::AddAttribute(const std::string& name, const std::string& value)
{
  m_tree_data.AddAttribute(name, value);
}

bool TreeDataComposerComponent::HasNextComponent() const
{
  return !m_component_queue.empty();
}

std::size_t TreeDataComposerComponent::GetQueuedComponentsCount() const
{
  return m_component_queue.size();
}

void TreeDataComposerComponent::AddNextComponent(component_t component)
{
  m_component_queue.emplace_back(std::move(component));
}

TreeDataComposerComponent::component_t TreeDataComposerComponent::TakeNextComponent()
{
  auto popped = std::move(m_component_queue.front());
  m_component_queue.pop_front();
  return popped;
}

}  // namespace mvvm
