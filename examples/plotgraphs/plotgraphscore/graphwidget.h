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

namespace mvvm {
class ItemsTreeView;
class GraphCanvas;
} // namespace ModelView

namespace PlotGraphs {

class GraphModel;
class GraphPropertyWidget;

//! Shows canvas with plots on the left and property editor on the right.

class GraphWidget : public QWidget {
    Q_OBJECT

public:
    explicit GraphWidget(GraphModel* model = nullptr, QWidget* parent = nullptr);
    ~GraphWidget() override;

    void setModel(GraphModel* model);

private:
    void initActions();
    QBoxLayout* createLeftLayout();
    QBoxLayout* createRightLayout();

    QToolBar* m_toolBar{nullptr};
    QAction* m_resetViewportAction{nullptr};
    QAction* m_addGraphAction{nullptr};
    QAction* m_removeGraphAction{nullptr};
    mvvm::GraphCanvas* m_graphCanvas{nullptr};
    GraphPropertyWidget* m_propertyWidget{nullptr};
    GraphModel* m_model{nullptr};
};

} // namespace PlotGraphs

#endif // PLOTGRAPHSCORE_GRAPHWIDGET_H
