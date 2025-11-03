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

#ifndef MVVM_NODEEDITOR_PAINTING_NODE_CONNECTION_PAINTER_H_
#define MVVM_NODEEDITOR_PAINTING_NODE_CONNECTION_PAINTER_H_

#include <mvvm/nodeeditor/node_connection_style.h>
#include <mvvm/nodeeditor/node_editor_types.h>

class QStyleOptionGraphicsItem;
class QPainter;

namespace mvvm
{

/**
 * @brief The NodeConnectionPainter class assists NodeConnectionShape in painting.
 */
class NodeConnectionPainter
{
public:
  NodeConnectionPainter() = default;
  ~NodeConnectionPainter() = default;

  NodeConnectionPainter(const NodeConnectionPainter&) = delete;
  NodeConnectionPainter& operator=(const NodeConnectionPainter&) = delete;
  NodeConnectionPainter(NodeConnectionPainter&&) = delete;
  NodeConnectionPainter& operator=(NodeConnectionPainter&&) = delete;

  void Paint(QPainter* painter, const QStyleOptionGraphicsItem* option);

private:
  NodeConnectionStyle m_shape_style;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PAINTING_NODE_CONNECTION_PAINTER_H_
