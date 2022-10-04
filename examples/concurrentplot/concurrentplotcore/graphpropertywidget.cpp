// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "graphpropertywidget.h"

#include "graphmodel.h"
#include "mvvm/widgets/all_items_tree_view.h"

#include <QBoxLayout>

using namespace mvvm;

GraphPropertyWidget::GraphPropertyWidget(GraphModel* model, QWidget* parent)
    : QWidget(parent), m_treeView(new AllItemsTreeView)
{
  auto layout = new QVBoxLayout;
  layout->addWidget(m_treeView);
  setLayout(layout);
  setModel(model);
}

GraphPropertyWidget::~GraphPropertyWidget() = default;

void GraphPropertyWidget::setModel(GraphModel* model)
{
  if (!model)
    return;
}
