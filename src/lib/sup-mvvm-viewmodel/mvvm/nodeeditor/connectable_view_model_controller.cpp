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
#include "graphics_scene_helper.h"
#include "i_connectable_shape_factory.h"
#include "node_connection_shape.h"

#include <mvvm/providers/i_children_strategy.h>
#include <mvvm/signals/model_listener.h>
#include <mvvm/utils/container_utils.h>

#include <QGraphicsScene>
#include <stack>

namespace mvvm
{

namespace
{

/**
 * @brief The Node class intended to store the pair item/shape in stack-based algorithms.
 */
struct Node
{
  SessionItem *item{nullptr};
  const ConnectableShape *shape{nullptr};
};

/**
 * @brief Returns a list of connection shapes that are not connected to two ports.
 */
std::vector<NodeConnectionShape *> FindHangingConnectionShapes(QGraphicsScene *scene)
{
  auto connections = mvvm::GetShapes<NodeConnectionShape>(*scene);
  std::vector<NodeConnectionShape *> result;
  std::copy_if(connections.begin(), connections.end(), std::back_inserter(result),
               [](auto connection) { return connection->IsHanging(); });
  return result;
}

}  // namespace

ConnectableViewModelController::ConnectableViewModelController(
    std::unique_ptr<IConnectableShapeFactory> factory,
    std::unique_ptr<IChildrenStrategy> children_strategy, QGraphicsScene *graphics_scene)
    : m_shape_factory(std::move(factory))
    , m_children_strategy(std::move(children_strategy))
    , m_graphics_scene(graphics_scene)
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

  if (!m_children_strategy)
  {
    throw RuntimeException(
        "Attempt to initialize view model controller with invalid children strategy");
  }
}

ConnectableViewModelController::~ConnectableViewModelController() = default;

void ConnectableViewModelController::OnModelEvent(const ItemInsertedEvent &event)
{
  auto [parent, tag_index] = event;

  if (IsInScope(parent))
  {
    auto new_child = parent->GetItem<mvvm::SessionItem>(tag_index);

    if (IsChildNeedsView(*parent, *new_child))
    {
      auto parent_shape = m_item_view_map.FindView(parent);
      PopulateScene(new_child, parent_shape);
    }
  }
}

void ConnectableViewModelController::OnModelEvent(const AboutToRemoveItemEvent &event)
{
  auto [parent, tag_index] = event;
  auto item_to_remove = parent->GetItem(tag_index);

  if (IsAboutToDestroyRootItem(item_to_remove))
  {
    // We can't continue if our root container is being removed.
    m_graphics_scene->clear();
    m_item_view_map.Clear();
    m_listener.reset();
    return;
  }

  if (IsInScope(event.item))
  {
    // remove views
    auto shapes_to_remove = m_item_view_map.RemoveRecursively(item_to_remove);
    for (auto shape : shapes_to_remove)
    {
      m_graphics_scene->removeItem(shape);
      delete shape;
    }

    // remove hanging connections
    for (auto connection : FindHangingConnectionShapes(m_graphics_scene))
    {
      m_graphics_scene->removeItem(connection);
      delete connection;
    }
  }
}

void ConnectableViewModelController::InitScene(mvvm::SessionItem *container_item)
{
  if (!container_item)
  {
    throw RuntimeException("Attempt to initialize view model controller with invalid item");
  }

  m_root_item = container_item;

  ConnectToModel(m_root_item->GetModel());

  m_graphics_scene->clear();
  m_item_view_map.Clear();
  for (auto item : m_children_strategy->GetChildren(m_root_item))
  {
    PopulateScene(item, nullptr);
  }
}

ConnectableShape *ConnectableViewModelController::FindShapeForItem(const SessionItem &item) const
{
  return m_item_view_map.FindView(&item);
}

void ConnectableViewModelController::UpdateBranch(const SessionItem *item)
{
  auto tag_index = item->GetTagIndex();
  auto parent = item->GetParent();

  // we pretend that this item was removed
  OnModelEvent(AboutToRemoveItemEvent{parent, tag_index});

  // we pretend that this item was inserted
  OnModelEvent(mvvm::ItemInsertedEvent{parent, tag_index});

  // If there was a change in children (as reported by children strategy), all views will be
  // regenerated. This will effectively hide children shapes for "collapsed" parents, and show
  // children shapes for "expanded" parents.
}

ModelListener *ConnectableViewModelController::Listener() const
{
  return m_listener.get();
}

const SessionItem *ConnectableViewModelController::FindItemForShape(ConnectableShape *shape) const
{
  return m_item_view_map.FindItem(shape);
}

SessionItem *ConnectableViewModelController::GetRootItem() const
{
  return m_root_item;
}

ConnectableViewModelController::item_view_map_t &ConnectableViewModelController::GetItemViewMap()
{
  return m_item_view_map;
}

void ConnectableViewModelController::PopulateScene(SessionItem *top_level_node_item,
                                                   const ConnectableShape *parent_shape)
{
  std::stack<Node> stack;

  stack.push({top_level_node_item, parent_shape});

  while (!stack.empty())
  {
    auto current_item = stack.top().item;
    auto current_parent_shape = stack.top().shape;
    stack.pop();

    auto new_shape = m_shape_factory->CreateShape(current_item);
    if (!new_shape)
    {
      continue;
    }
    auto new_shape_ptr = new_shape.get();

    m_graphics_scene->addItem(new_shape.release());  // transfer ownership to the scene

    m_item_view_map.Insert(current_item, new_shape_ptr);

    if (current_parent_shape)
    {
      current_parent_shape->MakeChildConnected(new_shape_ptr);
    }

    auto child_items = m_children_strategy->GetChildren(current_item);
    // visited in reverse order to populate the stack properly
    for (auto it = child_items.rbegin(); it != child_items.rend(); ++it)
    {
      stack.push({*it, new_shape_ptr});
    }
  }
}

void ConnectableViewModelController::ConnectToModel(ISessionModel *model)
{
  m_listener = std::make_unique<mvvm::ModelListener>(model);

  m_listener->Connect<mvvm::ItemInsertedEvent>(this, &ConnectableViewModelController::OnModelEvent);
  m_listener->Connect<mvvm::AboutToRemoveItemEvent>(this,
                                                    &ConnectableViewModelController::OnModelEvent);
}

bool ConnectableViewModelController::IsInScope(const mvvm::SessionItem *parent_item) const
{
  // the parent should be either our container or one of its children down in the hierarchy
  return parent_item == m_root_item || mvvm::utils::IsItemAncestor(parent_item, GetRootItem());
}

bool ConnectableViewModelController::IsAboutToDestroyRootItem(SessionItem *item_to_remove) const
{
  // the removal should not be related to root item or items above it
  return item_to_remove == GetRootItem() || utils::IsItemAncestor(GetRootItem(), item_to_remove);
}

bool ConnectableViewModelController::IsChildNeedsView(const SessionItem &parent,
                                                      const SessionItem &child)
{
  // We check that a child is among children reported by the strategy. If it is not the case, the
  // controller will ignore such children.
  auto children = m_children_strategy->GetChildren(&parent);
  return utils::Contains(children, &child);
}

}  // namespace mvvm
