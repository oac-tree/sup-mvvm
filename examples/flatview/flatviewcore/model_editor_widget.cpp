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


#include "model_editor_widget.h"

#include "sample_model.h"

#include <mvvm/delegates/viewmodel_delegate.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/viewmodel/property_table_viewmodel.h>

#include <QBoxLayout>
#include <QHeaderView>
#include <QTableView>
#include <QTreeView>

namespace flatview
{

ModelEditorWidget::ModelEditorWidget(SampleModel* model, QWidget* parent)
    : QWidget(parent)
    , m_vertical_tree(new QTreeView)
    , m_horizontal_tree(new QTreeView)
    , m_table_view(new QTableView)
    , m_delegate(std::make_unique<mvvm::ViewModelDelegate>())
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
  m_vertical_view_model = std::make_unique<mvvm::AllItemsViewModel>(model);
  m_vertical_tree->setModel(m_vertical_view_model.get());
  m_vertical_tree->setItemDelegate(m_delegate.get());
  m_vertical_tree->expandAll();
  m_vertical_tree->resizeColumnToContents(0);
  m_vertical_tree->setRootIsDecorated(true);

  // setting up right tree
  m_horizontal_view_model = std::make_unique<mvvm::PropertyTableViewModel>(model);

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
