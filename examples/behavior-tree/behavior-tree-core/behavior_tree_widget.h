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

#ifndef BEHAVIOR_TREE_BEHAVIOR_TREE_CORE_BEHAVIOR_TREE_WIDGET_H_
#define BEHAVIOR_TREE_BEHAVIOR_TREE_CORE_BEHAVIOR_TREE_WIDGET_H_

#include <QWidget>
#include <memory>

namespace mvvm
{
class ApplicationModel;
class ConnectableViewModelController;
class NodeConnectionGuide;
}  // namespace mvvm

namespace behaviortree
{

class GraphicsScene;
class GraphicsView;

class BehaviorTreeWidget : public QWidget
{
  Q_OBJECT

public:
  explicit BehaviorTreeWidget(QWidget* parent_widget = nullptr);
  ~BehaviorTreeWidget() override;

private:
  GraphicsScene* m_graphics_scene{nullptr};
  GraphicsView* m_graphics_view{nullptr};
  std::unique_ptr<mvvm::ConnectableViewModelController> CreateController();

  std::unique_ptr<mvvm::ApplicationModel> m_model;
  std::unique_ptr<mvvm::ConnectableViewModelController> m_model_scene_controller;
  std::unique_ptr<mvvm::NodeConnectionGuide> m_connection_guide;
};

}  // namespace behaviortree

#endif  // BEHAVIOR_TREE_BEHAVIOR_TREE_CORE_BEHAVIOR_TREE_WIDGET_H_
