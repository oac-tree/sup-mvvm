/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <mvvm/model/model_utils.h>
#include <mvvm/plotting/graph_canvas.h>
#include <mvvm/standarditems/graph_viewport_item.h>
#include <mvvm/widgets/all_items_tree_view.h>

#include <QBoxLayout>

namespace concurrentplot
{

GraphWidget::GraphWidget(GraphModel* model, QWidget* parent)
    : QWidget(parent)
    , m_toolbar(new GraphWidgetToolBar)
    , m_graph_canvas(new mvvm::GraphCanvas)
    , m_tree_view(new mvvm::AllItemsTreeView)
    , m_job_manager(new JobManager(this))
{
  auto main_layout = new QVBoxLayout;
  main_layout->setSpacing(10);

  auto central_layout = new QHBoxLayout;
  central_layout->addWidget(m_graph_canvas, 3);
  central_layout->addWidget(m_tree_view, 1);
  main_layout->addWidget(m_toolbar);
  main_layout->addLayout(central_layout);
  setLayout(main_layout);

  SetModel(model);
  InitToolbarConnections();
  InitJobmanagerConnections();
}

void GraphWidget::SetModel(GraphModel* model)
{
  if (!model)
  {
    return;
  }
  m_model = model;
  m_tree_view->SetApplicationModel(m_model);
  m_graph_canvas->SetItem(mvvm::utils::GetTopItem<mvvm::GraphViewportItem>(m_model));
}

//! Takes simulation results from JobManager and write into the model.

void GraphWidget::OnSimulationCompleted()
{
  auto data = m_job_manager->GetSimulationResult();
  if (!data.empty())
  {
    m_model->SetData(data);
  }
}

//! Connects signals going from toolbar.

void GraphWidget::InitToolbarConnections()
{
  // Change in amplitude is propagated from toolbar to JobManager.
  connect(m_toolbar, &GraphWidgetToolBar::valueChanged, m_job_manager,
          &JobManager::RequestSimulation);

  // simulation delay factor is propagated from toolbar to JobManager
  connect(m_toolbar, &GraphWidgetToolBar::delayChanged, m_job_manager, &JobManager::SetDelay);

  // cancel click is propagated from toolbar to JobManager
  connect(m_toolbar, &GraphWidgetToolBar::cancelPressed, m_job_manager,
          &JobManager::OnInterruptRequest);
}

//! Connect signals going from JobManager.
//! Connections are made queued since signals are emitted from non-GUI thread and we want to
//! deal with widgets.

void GraphWidget::InitJobmanagerConnections()
{
  // Simulation progress is propagated from JobManager to toolbar.
  connect(m_job_manager, &JobManager::progressChanged, m_toolbar,
          &GraphWidgetToolBar::OnProgressChanged, Qt::QueuedConnection);

  // Notification about completed simulation from jobManager to GraphWidget.
  connect(m_job_manager, &JobManager::simulationCompleted, this,
          &GraphWidget::OnSimulationCompleted, Qt::QueuedConnection);
}

}  // namespace concurrentplot
