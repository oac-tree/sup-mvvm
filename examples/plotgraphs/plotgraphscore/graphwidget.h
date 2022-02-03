// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef PLOTGRAPHSCORE_GRAPHWIDGET_H
#define PLOTGRAPHSCORE_GRAPHWIDGET_H

#include <QWidget>
#include <memory>

class QBoxLayout;
class QToolBar;
class QAction;

namespace mvvm
{
class ItemsTreeView;
class GraphCanvas;
class AllItemsTreeView;
}  // namespace mvvm

namespace plotgraphs
{

class GraphModel;
class GraphPropertyWidget;

//! Shows canvas with plots on the left and property editor on the right.

class GraphWidget : public QWidget
{
  Q_OBJECT

public:
  explicit GraphWidget(GraphModel* model = nullptr, QWidget* parent = nullptr);
  ~GraphWidget() override;

  void setModel(GraphModel* model);

private:
  void initActions();
  QBoxLayout* createLeftLayout();
  QBoxLayout* createRightLayout();

  QToolBar* m_tool_bar{nullptr};
  QAction* m_reset_viewport_action{nullptr};
  QAction* m_add_graph_action{nullptr};
  QAction* m_remove_graph_action{nullptr};
  mvvm::GraphCanvas* m_graph_canvas{nullptr};
  mvvm::AllItemsTreeView* m_propertyWidget{nullptr};
  GraphModel* m_model{nullptr};
};

}  // namespace plotgraphs

#endif  // PLOTGRAPHSCORE_GRAPHWIDGET_H
