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

#ifndef PLOTGRAPHS_PLOTGRAPHSCORE_GRAPH_WIDGET_H_
#define PLOTGRAPHS_PLOTGRAPHSCORE_GRAPH_WIDGET_H_

#include <QWidget>

class QBoxLayout;
class QToolBar;
class QAction;

namespace mvvm
{
class AllItemsTreeView;
class ChartCanvas;
}  // namespace mvvm

namespace lineseries
{

class GraphModel;

//! Shows canvas with plots on the left and property editor on the right.

class GraphWidget : public QWidget
{
  Q_OBJECT

public:
  explicit GraphWidget(GraphModel* model = nullptr, QWidget* parent = nullptr);
  ~GraphWidget() override;

  void SetModel(GraphModel* model);

private:
  void InitActions();

  QToolBar* m_tool_bar{nullptr};
  QAction* m_reset_viewport_action{nullptr};
  QAction* m_add_graph_action{nullptr};
  QAction* m_remove_graph_action{nullptr};
  QAction* m_randomize_action{nullptr};

  mvvm::ChartCanvas* m_chart_canvas{nullptr};
  mvvm::AllItemsTreeView* m_tree_view{nullptr};

  GraphModel* m_model{nullptr};
};

}  // namespace lineseries

#endif  // PLOTGRAPHS_PLOTGRAPHSCORE_GRAPH_WIDGET_H_
