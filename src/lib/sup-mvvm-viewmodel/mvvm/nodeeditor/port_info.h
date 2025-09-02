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

#ifndef MVVM_NODEEDITOR_PORT_INFO_H_
#define MVVM_NODEEDITOR_PORT_INFO_H_

#include <string>

namespace mvvm
{

/**
 * @brief The PortInfo contains a simple information about the node port.
 *
 * Ports of the same type can be connected together.
 */
class PortInfo
{
public:
  /**
   * @brief Initiazes a PortInfo with the given type and color.
   */
  PortInfo(const std::string& port_type, const std::string& port_color);

  /**
   * @brief Returns a string representing a port type.
   *
   * Ports of the same type can be connected together.
   */
  std::string GetPortType() const;

  /**
   * @brief Returns a string representing a port color.
   *
   * This should be the name which QColor understands,
   * i.e. https://www.w3.org/TR/css-color-3/#svg-color, or any "#RRGGBB" format.
   */
  std::string GetPortColor() const;

private:
  std::string m_port_type;
  std::string m_port_color;
};

/**
 * @brief Returns a default PortInfo instance.
 */
const PortInfo& GetDefaultPortInfo();

/**
 * @brief Returns a PortInfo instance for the root port.
 */
const PortInfo& GetRootPortInfo();

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PORT_INFO_H_
