// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "modeleditorwidget.h"

#include "samplemodel.h"

#include "mvvm/delegates/viewmodeldelegate.h"
#include "mvvm/viewmodel/allitemsviewmodel.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QTableView>
#include <QTreeView>

using namespace mvvm;

namespace celleditors
{

ModelEditorWidget::ModelEditorWidget(SampleModel* model, QWidget* parent)
    : QWidget(parent)
    , m_vertical_tree(new QTreeView)
    , m_horizontal_tree(new QTreeView)
    , m_table_view(new QTableView)
    , m_delegate(std::make_unique<ViewModelDelegate>())
{
  auto mainLayout = new QHBoxLayout;
  mainLayout->setSpacing(10);

  mainLayout->addLayout(CreateLeftLayout(), 1);
  mainLayout->addLayout(CreateRightLayout(), 3);

  setLayout(mainLayout);
  SetModel(model);
}

void ModelEditorWidget::SetModel(SampleModel* model)
{
  if (!model)
  {
    return;
  }

  // setting up left tree
  m_vertical_view_model = std::make_unique<AllItemsViewModel>(model);
  m_vertical_tree->setModel(m_vertical_view_model.get());
  m_vertical_tree->setItemDelegate(m_delegate.get());
  m_vertical_tree->expandAll();
  m_vertical_tree->resizeColumnToContents(0);

  // setting up right tree
  m_horizontal_view_model = std::make_unique<AllItemsViewModel>(model);

  m_horizontal_tree->setModel(m_horizontal_view_model.get());
  m_horizontal_tree->setItemDelegate(m_delegate.get());
  m_horizontal_tree->expandAll();
  m_horizontal_tree->header()->setSectionResizeMode(QHeaderView::Stretch);

  // setting up right table
  m_table_view->setModel(m_horizontal_view_model.get());
  m_table_view->setItemDelegate(m_delegate.get());
  m_table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

ModelEditorWidget::~ModelEditorWidget() = default;

QBoxLayout* ModelEditorWidget::CreateLeftLayout()
{
  auto result = new QVBoxLayout;
  result->addWidget(m_vertical_tree);
  return result;
}

QBoxLayout* ModelEditorWidget::CreateRightLayout()
{
  auto result = new QVBoxLayout;
  result->addWidget(m_horizontal_tree);
  result->addWidget(m_table_view);
  return result;
}

}  // namespace celleditors
