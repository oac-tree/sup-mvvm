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

#ifndef CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPH_WIDGET_H_
#define CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPH_WIDGET_H_

#include <QWidget>

namespace mvvm
{
class GraphCanvas;
class AllItemsTreeView;
}  // namespace mvvm

namespace concurrentplot
{

class GraphModel;
class GraphWidgetToolBar;
class JobManager;

//! Shows canvas with plots on the left and property editor on the right.
//! Provides connections between toolbar on JobManager.

class GraphWidget : public QWidget
{
  Q_OBJECT

public:
  explicit GraphWidget(GraphModel* model = nullptr, QWidget* parent = nullptr);

  void SetModel(GraphModel* model);

public slots:
  void OnSimulationCompleted();

private:
  void InitToolbarConnections();
  void InitJobmanagerConnections();

  GraphWidgetToolBar* m_toolbar{nullptr};
  mvvm::GraphCanvas* m_graph_canvas{nullptr};
  mvvm::AllItemsTreeView* m_tree_view{nullptr};
  GraphModel* m_model{nullptr};
  JobManager* m_job_manager{nullptr};
};

}  // namespace concurrentplot

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPH_WIDGET_H_
