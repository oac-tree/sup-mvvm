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

#ifndef MVVM_NODEEDITOR_I_NODE_ITEM_CONNECTABLE_SHAPE_ADAPTER_H_
#define MVVM_NODEEDITOR_I_NODE_ITEM_CONNECTABLE_SHAPE_ADAPTER_H_

#include <mvvm/nodeeditor/i_connectable_shape_adapter.h>

#include <QColor>
#include <QString>
#include <vector>

namespace mvvm
{

class NodeItem;

/**
 * @brief The NodeItemConnectableShapeAdapter adapts NodeItem for needs of ConnectableShape.
 */
class NodeItemConnectableShapeAdapter : public IConnectableShapeAdapter
{
public:
  explicit NodeItemConnectableShapeAdapter(NodeItem* node_item);

  SessionItem* GetItem() const override;

  QString GetDisplayName() const override;

  QColor GetBaseColor() const override;

  double GetX() const override;

  void SetX(double value) override;

  double GetY() const override;

  void SetY(double value) override;

  std::vector<PortInfo> GetPortInfos() const override;

private:
  NodeItem* m_node_item{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_I_CONNECTABLE_SHAPE_ADAPTER_H_
