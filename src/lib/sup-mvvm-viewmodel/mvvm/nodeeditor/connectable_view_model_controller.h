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

/**
 * @brief The ConnectableViewModelController syncronizes a model with node items and objects on the
 * graphics scene.
 */
class ConnectableViewModelController
{
public:
  using item_view_map_t = ItemViewMap<SessionItem, ConnectableShape>;

  ConnectableViewModelController(std::unique_ptr<IConnectableShapeFactory> factory,
                                 QGraphicsScene* graphics_scene);

  virtual ~ConnectableViewModelController();

  ConnectableViewModelController(const ConnectableViewModelController& other) = delete;
  ConnectableViewModelController& operator=(const ConnectableViewModelController& other) = delete;
  ConnectableViewModelController(ConnectableViewModelController&& other) = delete;
  ConnectableViewModelController& operator=(ConnectableViewModelController&&) = delete;

  void OnModelEvent(const mvvm::ItemInsertedEvent& event);

  void OnModelEvent(const mvvm::AboutToRemoveItemEvent& event);

  void InitScene(mvvm::SessionItem* container_item);

  item_view_map_t& GetItemViewMap();

  mvvm::SessionItem* GetRootItem() const;

private:
  void PopulateScene(mvvm::SessionItem* top_level_node_item, const ConnectableShape* parent_shape);
  void ConnectToModel(mvvm::ISessionModel* model);

  /**
   * @brief Checks if the given event corresponds to the removal in the container.
   */
  bool IsInScope(const mvvm::AboutToRemoveItemEvent& event) const;

  bool IsInvalidItemToRemove(mvvm::SessionItem* item) const;

  std::unique_ptr<IConnectableShapeFactory> m_shape_factory;
  QGraphicsScene* m_graphics_scene{nullptr};
  item_view_map_t m_item_view_map;
  std::unique_ptr<mvvm::ModelListener> m_listener;
  mvvm::SessionItem *m_root_item{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_I_CONNECTABLE_VIEW_MODEL_CONTROLLER_H_
