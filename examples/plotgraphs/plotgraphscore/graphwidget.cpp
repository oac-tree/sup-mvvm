// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "graphwidget.h"

#include "graphmodel.h"
#include "graphpropertywidget.h"

#include "mvvm/plotting/graphcanvas.h"
#include "mvvm/standarditems/graphviewportitem.h"
#include "mvvm/widgets/allitemstreeview.h"

#include <QAction>
#include <QBoxLayout>
#include <QToolBar>
#include <QToolButton>


namespace plotgraphs
{

GraphWidget::GraphWidget(GraphModel* model, QWidget* parent)
    : QWidget(parent)
    , m_tool_bar(new QToolBar)
    , m_graph_canvas(new mvvm::GraphCanvas)
    , m_propertyWidget(new mvvm::AllItemsTreeView)
{
  auto mainLayout = new QVBoxLayout;
  mainLayout->setSpacing(10);

  auto centralLayout = new QHBoxLayout;

  centralLayout->addLayout(createLeftLayout(), 3);
  centralLayout->addLayout(createRightLayout(), 1);

  mainLayout->addWidget(m_tool_bar);
  mainLayout->addLayout(centralLayout);

  setLayout(mainLayout);
  setModel(model);

  initActions();
}

void GraphWidget::setModel(GraphModel* model)
{
  if (!model)
    return;

  m_model = model;

  m_propertyWidget->SetApplicationModel(model);

  m_graph_canvas->SetItem(model->GetTopItem<mvvm::GraphViewportItem>());
}

void GraphWidget::initActions()
{
  const int toolbar_icon_size = 24;
  m_tool_bar->setIconSize(QSize(toolbar_icon_size, toolbar_icon_size));

  m_reset_viewport_action = new QAction("Reset view", this);
  auto on_reset = [this]()
  {
    auto viewport = m_model->GetTopItem<mvvm::GraphViewportItem>();
    viewport->SetViewportToContent(0.0, 0.1, 0.0, 0.1);
  };
  connect(m_reset_viewport_action, &QAction::triggered, on_reset);

  m_add_graph_action = new QAction("Add graph", this);
  auto on_add_graph = [this]() { m_model->AddGraph(); };
  connect(m_add_graph_action, &QAction::triggered, on_add_graph);

  m_remove_graph_action = new QAction("Remove graph", this);
  auto on_remove_graph = [this]() { m_model->RemoveGraph(); };
  connect(m_remove_graph_action, &QAction::triggered, on_remove_graph);

  m_tool_bar->addAction(m_reset_viewport_action);
  m_tool_bar->addAction(m_add_graph_action);
  m_tool_bar->addAction(m_remove_graph_action);

  m_tool_bar->addSeparator();
}

GraphWidget::~GraphWidget() = default;

QBoxLayout* GraphWidget::createLeftLayout()
{
  auto result = new QVBoxLayout;
  result->addWidget(m_graph_canvas);
  return result;
}

QBoxLayout* GraphWidget::createRightLayout()
{
  auto result = new QVBoxLayout;
  result->addWidget(m_propertyWidget);
  return result;
}

}  // namespace plotgraphs
