/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "graph_widget.h"

#include "graph_model.h"

#include <mvvm/model/model_utils.h>
#include <mvvm/plotting/graph_canvas.h>
#include <mvvm/standarditems/graph_viewport_item.h>
#include <mvvm/widgets/all_items_tree_view.h>

#include <QAction>
#include <QBoxLayout>
#include <QToolBar>

namespace plotgraphs
{

GraphWidget::GraphWidget(GraphModel* model, QWidget* parent)
    : QWidget(parent)
    , m_tool_bar(new QToolBar)
    , m_graph_canvas(new mvvm::GraphCanvas)
    , m_tree_view(new mvvm::AllItemsTreeView)
{
  auto central_layout = new QHBoxLayout;
  central_layout->addWidget(m_graph_canvas, 3);
  central_layout->addWidget(m_tree_view, 1);

  auto main_layout = new QVBoxLayout;
  main_layout->addWidget(m_tool_bar);
  main_layout->addLayout(central_layout);
  setLayout(main_layout);

  InitActions();

  SetModel(model);
}

GraphWidget::~GraphWidget() = default;

void GraphWidget::SetModel(GraphModel* model)
{
  if (!model)
  {
    return;
  }

  m_model = model;
  m_tree_view->SetApplicationModel(model);
  m_graph_canvas->SetItem(::mvvm::utils::GetTopItem<mvvm::GraphViewportItem>(model));
}

void GraphWidget::InitActions()
{
  const int toolbar_icon_size = 24;
  m_tool_bar->setIconSize(QSize(toolbar_icon_size, toolbar_icon_size));

  // reset view action
  m_reset_viewport_action = new QAction("Reset view", this);
  auto on_reset = [this]() { m_model->GetViewport()->SetViewportToContent(0.0, 0.1, 0.0, 0.1); };
  connect(m_reset_viewport_action, &QAction::triggered, on_reset);
  m_tool_bar->addAction(m_reset_viewport_action);

  // add graph action
  m_add_graph_action = new QAction("Add graph", this);
  auto on_add_graph = [this]() { m_model->AddGraph(); };
  connect(m_add_graph_action, &QAction::triggered, on_add_graph);
  m_tool_bar->addAction(m_add_graph_action);

  // remove graph action
  m_remove_graph_action = new QAction("Remove graph", this);
  auto on_remove_graph = [this]() { m_model->RemoveGraph(); };
  connect(m_remove_graph_action, &QAction::triggered, on_remove_graph);
  m_tool_bar->addAction(m_remove_graph_action);

  // randomize graph action
  m_randomize_action = new QAction("Randomize graph", this);
  auto on_randomize = [this]() { m_model->RandomizeGraphs(); };
  connect(m_randomize_action, &QAction::triggered, on_randomize);
  m_tool_bar->addAction(m_randomize_action);
}

}  // namespace plotgraphs
