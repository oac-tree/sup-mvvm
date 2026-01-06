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

#ifndef MVVM_NODEEDITOR_PORT_INFO_H_
#define MVVM_NODEEDITOR_PORT_INFO_H_

#include <mvvm/nodeeditor/node_editor_types.h>

#include <string>

namespace mvvm
{

class SessionItem;

/**
 * @brief The PortInfo contains an information about the node port related to the connection logic.
 *
 * This information comes from the model and is unrelated to how the port is painted.
 *
 * Ports of the same type can be connected together if their directions matches (input can be
 * connected with output).
 */
class PortInfo
{
public:
  /**
   * @brief Initiazes a PortInfo with the given direction and type.
   */
  PortInfo(PortDirection port_direction, std::string port_type);

  /**
   * @brief Initializes a PortInfo with all available info.
   *
   * @param port_direction Direction of the port (input or output).
   * @param port_type A string representing a port type.
   * @param port_label A label for the port.
   * @param origin An origin item for this port, if any.
   */
  PortInfo(PortDirection port_direction, std::string port_type, std::string port_label,
           SessionItem* origin);

  /**
   * @brief Returns port direction.
   */
  PortDirection GetPortDirection() const;

  /**
   * @brief Returns a string representing a port type.
   *
   * Ports of the same type can be connected together if their directions matches (input can be
   * connected with output).
   */
  std::string GetPortType() const;

  /**
   * @brief Returns a port label.
   */
  std::string GetPortLabel() const;

  /**
   * @brief Returns an origin item for this port, if any.
   */
  mvvm::SessionItem* GetOrigin() const;

private:
  PortDirection m_port_direction;
  std::string m_port_type;
  std::string m_port_label;
  SessionItem* m_origin{nullptr};
};

bool operator==(const PortInfo& lhs, const PortInfo& rhs);
bool operator!=(const PortInfo& lhs, const PortInfo& rhs);

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PORT_INFO_H_
