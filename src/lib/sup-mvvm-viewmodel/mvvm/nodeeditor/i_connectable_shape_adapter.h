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

#ifndef MVVM_NODEEDITOR_I_CONNECTABLE_SHAPE_ADAPTER_H_
#define MVVM_NODEEDITOR_I_CONNECTABLE_SHAPE_ADAPTER_H_

#include <mvvm/nodeeditor/connectable_shape_style.h>
#include <mvvm/nodeeditor/node_editor_events.h>
#include <mvvm/nodeeditor/port_info.h>

#include <QColor>
#include <QString>
#include <vector>

namespace mvvm
{

class SessionItem;

/**
 * @brief The IConnectableShapeAdapter allows to adapt custom object for plotting needs.
 *
 * Its concrete implementation will allow ConnectableShape to talk with user implementation (i.e.
 * InstructionItem).
 */
class IConnectableShapeAdapter
{
public:
  IConnectableShapeAdapter() = default;
  virtual ~IConnectableShapeAdapter() = default;

  IConnectableShapeAdapter(const IConnectableShapeAdapter&) = delete;
  IConnectableShapeAdapter& operator=(const IConnectableShapeAdapter&) = delete;
  IConnectableShapeAdapter(IConnectableShapeAdapter&&) = delete;
  IConnectableShapeAdapter& operator=(IConnectableShapeAdapter&&) = delete;

  /**
   * @brief Returns the item associated with the connectable element.
   */
  virtual SessionItem* GetItem() const = 0;

  /**
   * @brief Returns a string to be used as a label for the connectable element.
   */
  virtual QString GetDisplayName() const = 0;

  /**
   * @brief Returns X coordinate of the connectable element.
   */
  virtual double GetX() const = 0;

  /**
   * @brief Sets X coordinate of the connectable element.
   */
  virtual void SetX(double value) = 0;

  /**
   * @brief Returns Y coordinate of the connectable element.
   */
  virtual double GetY() const = 0;

  /**
   * @brief Sets Y coordinate of the connectable element.
   */
  virtual void SetY(double value) = 0;

  /**
   * @brief Returns a list of port infos to be used for creating ports on the connectable element.
   */
  virtual std::vector<PortInfo> GetPortInfos() const = 0;

  /**
   * @brief Returns operation state of the connectable element.
   */
  virtual NodeOperationStates GetOperationStates() const = 0;

  /**
   * @brief Processes user interface events reported by various shapes composing the node tree.
   */
  virtual void ProcessEvent(const node_event_t& event) const = 0;

  /**
   * @brief Returns style information for the connectable shape.
   *
   * To think about: the style can be heavy object, should we return by value or by reference?
   */
  virtual ConnectableShapeStyle GetStyle() const = 0;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_I_CONNECTABLE_SHAPE_ADAPTER_H_
