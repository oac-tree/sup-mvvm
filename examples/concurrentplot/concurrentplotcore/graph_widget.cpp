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
#include "graph_widget_toolbar.h"
#include "job_manager.h"
#include "mvvm/model/model_utils.h"
#include "mvvm/plotting/graph_canvas.h"
#include "mvvm/standarditems/graph_viewport_item.h"
#include "mvvm/widgets/all_items_tree_view.h"

#include <QBoxLayout>

using namespace mvvm;

GraphWidget::GraphWidget(GraphModel* model, QWidget* parent)
    : QWidget(parent)
    , m_toolbar(new GraphWidgetToolBar)
    , m_graphCanvas(new GraphCanvas)
    , m_tree_view(new mvvm::AllItemsTreeView)
    , m_jobManager(new JobManager(this))
{
  auto mainLayout = new QVBoxLayout;
  mainLayout->setSpacing(10);

  auto centralLayout = new QHBoxLayout;
  centralLayout->addWidget(m_graphCanvas, 3);
  centralLayout->addWidget(m_tree_view, 1);
  mainLayout->addWidget(m_toolbar);
  mainLayout->addLayout(centralLayout);
  setLayout(mainLayout);

  setModel(model);
  init_toolbar_connections();
  init_jobmanager_connections();
}

void GraphWidget::setModel(GraphModel* model)
{
  if (!model)
  {
    return;
  }
  m_model = model;
  m_tree_view->SetApplicationModel(m_model);
  m_graphCanvas->SetItem(utils::GetTopItem<GraphViewportItem>(m_model));
}

//! Takes simulation results from JobManager and write into the model.

void GraphWidget::onSimulationCompleted()
{
  auto data = m_jobManager->simulationResult();
  if (!data.empty())
    m_model->set_data(data);
}

//! Connects signals going from toolbar.

void GraphWidget::init_toolbar_connections()
{
  // Change in amplitude is propagated from toolbar to JobManager.
  connect(m_toolbar, &GraphWidgetToolBar::valueChanged, m_jobManager,
          &JobManager::requestSimulation);

  // simulation delay factor is propagated from toolbar to JobManager
  connect(m_toolbar, &GraphWidgetToolBar::delayChanged, m_jobManager, &JobManager::setDelay);

  // cancel click is propagated from toolbar to JobManager
  connect(m_toolbar, &GraphWidgetToolBar::cancelPressed, m_jobManager,
          &JobManager::onInterruptRequest);
}

//! Connect signals going from JobManager.
//! Connections are made queued since signals are emitted from non-GUI thread and we want to
//! deal with widgets.

void GraphWidget::init_jobmanager_connections()
{
  // Simulation progress is propagated from JobManager to toolbar.
  connect(m_jobManager, &JobManager::progressChanged, m_toolbar,
          &GraphWidgetToolBar::onProgressChanged, Qt::QueuedConnection);

  // Notification about completed simulation from jobManager to GraphWidget.
  connect(m_jobManager, &JobManager::simulationCompleted, this, &GraphWidget::onSimulationCompleted,
          Qt::QueuedConnection);
}
