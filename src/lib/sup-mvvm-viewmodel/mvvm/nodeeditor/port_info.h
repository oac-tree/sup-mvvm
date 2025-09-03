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

#include <cstdint>
#include <string>

namespace mvvm
{

/**
 * @brief The PortDirection enum introduces port's direction types so we know what can be connected
 * with what.
 *
 * Example: for behavioral tree with top-to-bottom layout
 * Consider parent "Sequence" instruction with a child "Wait" beneath
 *   kInput port is located on a Sequence instruction at its bottom.
 *   kOutput port is located on a Wait instruction at its top.
 */
enum class PortDirection : std::uint8_t
{
  kDefault,
  kInput,
  kOutput
};

/**
 * @brief The PortInfo contains a simple information about the node port.
 *
 * Ports of the same type can be connected together.
 */
class PortInfo
{
public:
  /**
   * @brief Initiazes a PortInfo with the given direction, type and color.
   */
  PortInfo(PortDirection port_direction, std::string port_type, std::string port_color);

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
   * @brief Returns a string representing a port color.
   *
   * This should be the name which QColor understands,
   * i.e. https://www.w3.org/TR/css-color-3/#svg-color, or any "#RRGGBB" format.
   */
  std::string GetPortColor() const;

private:
  PortDirection m_port_direction;
  std::string m_port_type;
  std::string m_port_color;
};

bool operator==(const PortInfo& lhs, const PortInfo& rhs);
bool operator!=(const PortInfo& lhs, const PortInfo& rhs);

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PORT_INFO_H_
