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

#ifndef MVVM_NODEEDITOR_I_CONNECTABLE_VIEW_MODEL_CONTROLLER_H_
#define MVVM_NODEEDITOR_I_CONNECTABLE_VIEW_MODEL_CONTROLLER_H_

#include <mvvm/providers/item_view_map.h>
#include <mvvm/signals/event_types.h>

class QGraphicsScene;

namespace mvvm
{

class IConnectableShapeFactory;
class ISessionModel;
class ConnectableShape;
class ModelListener;
class IChildrenStrategy;

/**
 * @brief The ConnectableViewModelController syncronizes a model with node items and objects on the
 * graphics scene.
 */
class ConnectableViewModelController
{
public:
  using item_view_map_t = ItemViewMap<const SessionItem*, ConnectableShape*>;

  ConnectableViewModelController(std::unique_ptr<IConnectableShapeFactory> factory,
                                 std::unique_ptr<IChildrenStrategy> children_strategy,
                                 QGraphicsScene* graphics_scene);

  virtual ~ConnectableViewModelController();

  ConnectableViewModelController(const ConnectableViewModelController& other) = delete;
  ConnectableViewModelController& operator=(const ConnectableViewModelController& other) = delete;
  ConnectableViewModelController(ConnectableViewModelController&& other) = delete;
  ConnectableViewModelController& operator=(ConnectableViewModelController&&) = delete;

  void OnModelEvent(const mvvm::ItemInsertedEvent& event);

  void OnModelEvent(const mvvm::AboutToRemoveItemEvent& event);

  void InitScene(mvvm::SessionItem* container_item);

  item_view_map_t& GetItemViewMap() &;

  mvvm::SessionItem* GetRootItem() const;

  ConnectableShape* FindShapeForItem(const SessionItem& item) const;

  const mvvm::SessionItem* FindItemForShape(ConnectableShape* shape) const;

  /**
   * @brief Regenerate all children views related to the given item.
   *
   * It will take into account all possible change in collapse/expand status. Original item's shape
   * will remain intact.
   */
  void UpdateBranch(SessionItem* item);

  /**
   * @brief Returns underlying listener.
   */
  ModelListener* Listener() const;

private:
  void PopulateScene(mvvm::SessionItem* top_level_node_item, const ConnectableShape* parent_shape);
  void ConnectToModel(mvvm::ISessionModel* model);

  /**
   * @brief Checks if the given parent_item corresponds to the activity in our container.
   */
  bool IsInScope(const SessionItem* parent_item) const;

  /**
   * @brief Checks if the removal of item will lead to the removal of our root item.
   *
   * We don't want to continue if our root item is being removed. If so, we will clear the scene
   * and disconnect from the model.
   */
  bool IsAboutToDestroyRootItem(const SessionItem* item_to_remove) const;

  /**
   * @brief Checks if given children needs a view.
   */
  bool IsChildNeedsView(const mvvm::SessionItem& parent, const mvvm::SessionItem& child);

  std::unique_ptr<IConnectableShapeFactory> m_shape_factory;
  std::unique_ptr<IChildrenStrategy> m_children_strategy;
  QGraphicsScene* m_graphics_scene{nullptr};
  item_view_map_t m_item_view_map;
  std::unique_ptr<ModelListener> m_listener;
  SessionItem* m_root_item{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_I_CONNECTABLE_VIEW_MODEL_CONTROLLER_H_
