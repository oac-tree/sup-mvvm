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

#include <QColor>
#include <QRectF>
#include <QString>

namespace mvvm
{

/**
 * @brief The PortAppearance contains information related to the visual appearance of the port.
 */
class PortAppearance
{
public:
  /**
   * @brief Initiazes a PortAppearance with the given location, label and color.
   */
  PortAppearance(PortLocation port_location, const QString& port_label, const QColor& port_color);

  /**
   * @brief Initiazes a PortAppearance with the given location, label, color and port radius.
   */
  PortAppearance(PortLocation port_location, const QString& port_label, const QColor& port_color,
                 double port_radius);

  /**
   * @brief Returns location of the port.
   */
  PortLocation GetPortLocation() const;

  /**
   * @brief Returns a port label.
   */
  QString GetPortLabel() const;

  /**
   * @brief Returns a port color.
   */
  QColor GetPortColor() const;

  /**
   * @brief Returns a bounding rectangle for the port appearance.
   */
  QRectF GetBoundingRect() const;

  /**
   * @brief Returns port radius.
   */
  double GetPortRadius() const;

private:
  void CalculateBoundingRect();

  PortLocation m_port_location;
  QString m_port_label;
  QColor m_port_color;
  QRectF m_bounding_rect;
  double m_port_radius;
};

bool operator==(const PortAppearance& lhs, const PortAppearance& rhs);
bool operator!=(const PortAppearance& lhs, const PortAppearance& rhs);

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PORT_APPEARANCE_H_
