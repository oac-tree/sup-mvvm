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

#ifndef MVVM_NODEEDITOR_NOTIFYING_CONNECTABLE_SHAPE_ADAPTER_H_
#define MVVM_NODEEDITOR_NOTIFYING_CONNECTABLE_SHAPE_ADAPTER_H_

#include <mvvm/nodeeditor/i_connectable_shape_adapter.h>
#include <mvvm/signals/event_types.h>

#include <functional>
#include <memory>

namespace mvvm
{

class IConnectableShapeAdapter;
class NodeItem;
class ItemListener;

/**
 * @brief The NotifyingConnectableShapeAdapter is a decorator to syncroniz mutual updates of
 * QGraphicsItem and IConnectableShapeAdapter.
 *
 * It notifyes the client of any change of underlying node item. In this adapter we assume that all
 * calls to setters are coming from a view (QGraphicsItem). To avoid cyclic updates we block
 * notifications when we are updating the view.
 */
class NotifyingConnectableShapeAdapter : public IConnectableShapeAdapter
{
public:
  /**
   * @brief Initialize
   */
  NotifyingConnectableShapeAdapter(std::unique_ptr<IConnectableShapeAdapter> shape_adapter,
                                   const std::function<void()>& update_shape_callback);

  ~NotifyingConnectableShapeAdapter() override;

  NotifyingConnectableShapeAdapter(const NotifyingConnectableShapeAdapter&) = delete;
  NotifyingConnectableShapeAdapter& operator=(const NotifyingConnectableShapeAdapter&) = delete;
  NotifyingConnectableShapeAdapter(NotifyingConnectableShapeAdapter&&) = delete;
  NotifyingConnectableShapeAdapter& operator=(NotifyingConnectableShapeAdapter&&) = delete;

  SessionItem* GetItem() const override;
  QString GetDisplayName() const override;
  QColor GetBaseColor() const override;
  double GetX() const override;
  void SetX(double value) override;
  double GetY() const override;
  void SetY(double value) override;
  std::vector<PortInfo> GetPortInfos() const override;

private:
  void OnPropertyChanged(const PropertyChangedEvent& event);

  std::unique_ptr<IConnectableShapeAdapter> m_shape_adapter;
  std::function<void()> m_update_shape_callback;
  std::unique_ptr<ItemListener> m_item_listener;
  bool m_block_view_update{false};
  bool m_block_item_update{false};
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_NOTIFYING_CONNECTABLE_SHAPE_ADAPTER_H_
