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

#include "behavior_tree_widget.h"

#include "behavior_tree_items.h"
#include "graphics_scene.h"
#include "graphics_view.h"

#include <mvvm/model/application_model.h>
#include <mvvm/nodeeditor/connectable_shape.h>
#include <mvvm/nodeeditor/connectable_view_model_controller.h>
#include <mvvm/nodeeditor/default_node_shape_factory.h>
#include <mvvm/nodeeditor/i_node_port.h>
#include <mvvm/nodeeditor/node_connection_guide.h>
#include <mvvm/nodeeditor/node_editor_helper.h>
#include <mvvm/nodeeditor/node_item_connectable_shape_adapter.h>
#include <mvvm/nodeeditor/top_bottom_port_position_strategy.h>
#include <mvvm/providers/standard_children_strategies.h>
#include <mvvm/standarditems/node_item.h>

#include <QVBoxLayout>

Q_DECLARE_METATYPE(const mvvm::INodePort *)

namespace behaviortree
{

BehaviorTreeWidget::BehaviorTreeWidget(QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_graphics_scene(new GraphicsScene(this))
    , m_graphics_view(new GraphicsView(m_graphics_scene, this))
    , m_model(std::make_unique<mvvm::ApplicationModel>())
    , m_model_scene_controller(CreateController())
    , m_connection_guide(std::make_unique<mvvm::NodeConnectionGuide>(m_graphics_scene))
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_graphics_view);

  auto sequence = m_model->InsertItem<SequenceItem>();
  auto wait0 = m_model->InsertItem<WaitItem>(sequence);
  auto wait1 = m_model->InsertItem<WaitItem>();

  sequence->SetX(m_graphics_scene->sceneRect().center().x());
  sequence->SetY(m_graphics_scene->sceneRect().center().y());
  wait0->SetX(m_graphics_scene->sceneRect().center().x());
  wait0->SetY(m_graphics_scene->sceneRect().center().y() + 200);
  wait1->SetX(m_graphics_scene->sceneRect().center().x() + 200);
  wait1->SetY(m_graphics_scene->sceneRect().center().y() + 200);

  m_graphics_view->setRenderHint(QPainter::Antialiasing);

  SetupConnections();
}

BehaviorTreeWidget::~BehaviorTreeWidget() = default;

std::unique_ptr<mvvm::ConnectableViewModelController> BehaviorTreeWidget::CreateController()
{
  auto factory = std::make_unique<mvvm::DefaultNodeShapeFactory>();
  auto children_strategy = std::make_unique<mvvm::TopItemsStrategy>();
  auto result = std::make_unique<mvvm::ConnectableViewModelController>(
      std::move(factory), std::move(children_strategy), m_graphics_scene);
  result->InitScene(m_model->GetRootItem());
  return result;
}

void BehaviorTreeWidget::SetupConnections()
{
  auto on_connection_request = [this](auto start_port, auto end_port)
  { mvvm::ConnectPorts(start_port, end_port); };
  connect(m_connection_guide.get(), &mvvm::NodeConnectionGuide::connectionRequest, this,
          on_connection_request);

  connect(m_graphics_view, &GraphicsView::deleteSelectedRequest, m_graphics_scene,
          &GraphicsScene::OnDeleteSelectedRequest);
}

}  // namespace behaviortree
