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

#include "connectable_view_model_controller.h"

#include "connectable_shape.h"
#include "i_connectable_shape_factory.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/signals/model_listener.h>

#include <QGraphicsScene>
#include <stack>

namespace mvvm
{

namespace
{
struct Node
{
  SessionItem *item{nullptr};
  ConnectableShape *shape{nullptr};
};
}  // namespace

ConnectableViewModelController::ConnectableViewModelController(
    std::unique_ptr<IConnectableShapeFactory> factory, QGraphicsScene *graphics_scene)
    : m_shape_factory(std::move(factory)), m_graphics_scene(graphics_scene)
{
  if (!m_graphics_scene)
  {
    throw RuntimeException("Attempt to initialize view model controller with invalid scene");
  }

  if (!m_shape_factory)
  {
    throw RuntimeException(
        "Attempt to initialize view model controller with invalid shape factory");
  }
}

ConnectableViewModelController::~ConnectableViewModelController() = default;

void ConnectableViewModelController::OnModelEvent(const ItemInsertedEvent &event)
{
  (void)event;
}

void ConnectableViewModelController::OnModelEvent(const AboutToRemoveItemEvent &event)
{
  (void)event;
}

void ConnectableViewModelController::OnModelEvent(const DataChangedEvent &event)
{
  (void)event;
}

void ConnectableViewModelController::InitScene(mvvm::SessionItem *container_item)
{
  if (!container_item)
  {
    throw RuntimeException("Attempt to initialize view model controller with invalid item");
  }

  m_listener = std::make_unique<mvvm::ModelListener>(container_item->GetModel());

  m_graphics_scene->clear();
  m_item_view_map.Clear();
  for (auto item : utils::TopLevelItems(*container_item))
  {
    PopulateScene(item);
  }
}

void ConnectableViewModelController::PopulateScene(SessionItem *top_level_node_item)
{
  std::stack<Node> stack;

  stack.push({top_level_node_item, nullptr});

  while (!stack.empty())
  {
    auto current_parent_item = stack.top().item;
    auto current_parent_shape = stack.top().shape;
    stack.pop();

    auto new_shape = m_shape_factory->CreateShape(current_parent_item);
    auto new_shape_ptr = new_shape.get();

    m_graphics_scene->addItem(new_shape.release());  // transfer ownership to the scene

    m_item_view_map.Insert(current_parent_item, new_shape_ptr);

    if (current_parent_shape)
    {
      current_parent_shape->MakeChildConnected(new_shape_ptr);
    }

    auto child_items = utils::TopLevelItems(*current_parent_item);
    // visited in reverse order to populate the stack properly
    for (auto it = child_items.rbegin(); it != child_items.rend(); ++it)
    {
      stack.push({*it, new_shape_ptr});
    }
  }
}

}  // namespace mvvm
