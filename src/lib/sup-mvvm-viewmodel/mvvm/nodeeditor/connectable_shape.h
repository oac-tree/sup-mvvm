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

#ifndef MVVM_NODEEDITOR_CONNECTABLE_SHAPE_H_
#define MVVM_NODEEDITOR_CONNECTABLE_SHAPE_H_

#include <QGraphicsItem>
#include <memory>

namespace mvvm
{

class IConnectableShapeAdapter;

/**
 * @brief The ConnectableShape is a graphics object which have ports for connecting other objects.
 */
class ConnectableShape : public QGraphicsItem
{
public:
  explicit ConnectableShape(std::unique_ptr<IConnectableShapeAdapter> shape_adapter,
                   QGraphicsItem* parent_item = nullptr);
  ~ConnectableShape() override;

private:
  std::unique_ptr<IConnectableShapeAdapter> m_shape_adapter;
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_CONNECTABLE_SHAPE_H_
