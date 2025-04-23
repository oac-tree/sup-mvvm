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

#ifndef PLOTGRAPHS_PLOTGRAPHSCORE_GRAPH_WIDGET_H_
#define PLOTGRAPHS_PLOTGRAPHSCORE_GRAPH_WIDGET_H_

#include <QWidget>

class QBoxLayout;
class QToolBar;
class QAction;
class QSplitter;

namespace mvvm
{
class GraphCanvas;
class AllItemsTreeView;
}  // namespace mvvm

namespace plotgraphs
{

class GraphModel;

/**
 * @brief The GraphWidget class shows canvas with plots on the left and property editor on the
 * right.
 */
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
  QAction* m_undo_action{nullptr};
  QAction* m_redo_action{nullptr};

  mvvm::GraphCanvas* m_graph_canvas{nullptr};
  mvvm::AllItemsTreeView* m_tree_view{nullptr};
  QSplitter* m_splitter{nullptr};

  GraphModel* m_model{nullptr};
};

}  // namespace plotgraphs

#endif  // PLOTGRAPHS_PLOTGRAPHSCORE_GRAPH_WIDGET_H_
