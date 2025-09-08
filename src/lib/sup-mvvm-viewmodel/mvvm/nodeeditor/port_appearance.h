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

#ifndef MVVM_NODEEDITOR_PORT_APPEARANCE_H_
#define MVVM_NODEEDITOR_PORT_APPEARANCE_H_

#include <mvvm/nodeeditor/node_editor_types.h>

#include <string>

namespace mvvm
{

/**
 * @brief The PortAppearance contains information related to the visual appearance of the port.
 */
class PortAppearance
{
public:
  /**
   * @brief Initiazes a PortAppearance with the given location, type and color.
   */
  PortAppearance(PortLocation port_location, std::string port_label, std::string port_color);

  /**
   * @brief Returns location of the port.
   */
  PortLocation GetPortLocation() const;

  /**
   * @brief Returns a port label.
   */
  std::string GetPortLabel() const;

  /**
   * @brief Returns a string representing a port color.
   *
   * This should be the name which QColor understands,
   * i.e. https://www.w3.org/TR/css-color-3/#svg-color, or any "#RRGGBB" format.
   */
  std::string GetPortColor() const;

private:
  PortLocation m_port_location;
  std::string m_port_label;
  std::string m_port_color;
};

bool operator==(const PortAppearance& lhs, const PortAppearance& rhs);
bool operator!=(const PortAppearance& lhs, const PortAppearance& rhs);

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PORT_APPEARANCE_H_
