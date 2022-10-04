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

#ifndef CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHWIDGET_H
#define CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHWIDGET_H

#include <QWidget>

class GraphModel;
class GraphWidgetToolBar;
class JobManager;

namespace mvvm
{
class GraphCanvas;
class AllItemsTreeView;
}  // namespace ModelView

//! Shows canvas with plots on the left and property editor on the right.
//! Provides connections between toolbar on JobManager.

class GraphWidget : public QWidget
{
  Q_OBJECT

public:
  explicit GraphWidget(GraphModel* model = nullptr, QWidget* parent = nullptr);

  void setModel(GraphModel* model);

public slots:
  void onSimulationCompleted();

private:
  void init_toolbar_connections();
  void init_jobmanager_connections();

  GraphWidgetToolBar* m_toolbar{nullptr};
  mvvm::GraphCanvas* m_graphCanvas{nullptr};
  mvvm::AllItemsTreeView* m_tree_view{nullptr};
  GraphModel* m_model{nullptr};
  JobManager* m_jobManager{nullptr};
};

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_GRAPHWIDGET_H
