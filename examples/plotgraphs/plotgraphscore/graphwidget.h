/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef PLOTGRAPHSCORE_GRAPHWIDGET_H
#define PLOTGRAPHSCORE_GRAPHWIDGET_H

#include <QWidget>
#include <memory>

class QBoxLayout;
class QToolBar;
class QAction;

namespace mvvm
{
class GraphCanvas;
class AllItemsTreeView;
}  // namespace mvvm

namespace plotgraphs
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

  mvvm::GraphCanvas* m_graph_canvas{nullptr};
  mvvm::AllItemsTreeView* m_tree_view{nullptr};

  GraphModel* m_model{nullptr};
};

}  // namespace plotgraphs

#endif  // PLOTGRAPHSCORE_GRAPHWIDGET_H
