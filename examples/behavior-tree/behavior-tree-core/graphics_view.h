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

#ifndef BEHAVIOR_TREE_BEHAVIOR_TREE_CORE_GRAPHICS_VIEW_H_
#define BEHAVIOR_TREE_BEHAVIOR_TREE_CORE_GRAPHICS_VIEW_H_

#include <QGraphicsView>

namespace behaviortree
{

class GraphicsScene;

class GraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  explicit GraphicsView(GraphicsScene* graphics_scene, QWidget* parent_widget = nullptr);

signals:
  void deleteSelectedRequest();

protected:
  void keyPressEvent(QKeyEvent* event) override;
};

}  // namespace behaviortree

#endif  // BEHAVIOR_TREE_BEHAVIOR_TREE_CORE_GRAPHICS_VIEW_H_
