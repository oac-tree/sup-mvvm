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

#include "flat_widget.h"
#include "sample_model.h"

#include <mvvm/delegates/viewmodel_delegate.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/viewmodel/property_table_viewmodel.h>
#include <mvvm/viewmodel/property_viewmodel.h>

#include <QBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QTableView>
#include <QTreeView>

namespace flatview
{

ModelEditorWidget::ModelEditorWidget(SampleModel* model, QWidget* parent)
    : QWidget(parent)
    , m_vertical_tree(new QTreeView)
    , m_property_left_widget(new FlatWidget)
    , m_property_right_widget(new FlatWidget)
    , m_property_bottom_widget(new FlatWidget)
    , m_delegate(std::make_unique<mvvm::ViewModelDelegate>())
{
  auto mainLayout = new QHBoxLayout;
  mainLayout->setSpacing(10);

  mainLayout->addLayout(CreateLeftLayout(), 2);
  mainLayout->addLayout(CreateRightLayout(), 5);

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
  m_vertical_tree->resizeColumnToContents(0);
  m_vertical_tree->setRootIsDecorated(true);
  m_vertical_tree->expandAll();

  // setting up right tree
  m_horizontal_view_model = std::make_unique<mvvm::PropertyTableViewModel>(model);

  m_property_left_widget->SetViewModel(std::make_unique<mvvm::PropertyViewModel>(model));
  m_property_right_widget->SetViewModel(std::make_unique<mvvm::PropertyViewModel>(model));
  m_property_bottom_widget->SetViewModel(std::make_unique<mvvm::PropertyTableViewModel>(model));

  auto top_items = mvvm::utils::GetTopItems(model);
  m_property_left_widget->SetItem(top_items[0]);
  m_property_right_widget->SetItem(top_items[1]);
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

  auto box1_layout = new QHBoxLayout;
  box1_layout->addWidget(m_property_left_widget);
  auto box1 = new QGroupBox("Item0");
  box1->setLayout(box1_layout);

  auto box2_layout = new QHBoxLayout;
  box2_layout->addWidget(m_property_right_widget);
  auto box2 = new QGroupBox("Item1");
  box2->setLayout(box2_layout);

  auto hlayout = new QHBoxLayout;
  hlayout->addWidget(box1);
  hlayout->addWidget(box2);

  result->addLayout(hlayout);
  result->addSpacing(20);
  result->addWidget(m_property_bottom_widget);
  return result;
}

}  // namespace flatview
