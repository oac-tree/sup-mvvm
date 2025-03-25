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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "container_editor_widget.h"

#include "drag_viewmodel.h"
#include "sample_model.h"

#include <mvvm/model/model_utils.h>
#include <mvvm/views/viewmodel_delegate.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

#include <QBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTreeView>

namespace dragandmove
{

ContainerEditorWidget::ContainerEditorWidget(QWidget* parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_delegate(std::make_unique<mvvm::ViewModelDelegate>())
{
  auto mainLayout = new QVBoxLayout;
  mainLayout->setSpacing(10);

  mainLayout->addWidget(m_tree_view);
  mainLayout->addLayout(CreateButtonLayout());

  setLayout(mainLayout);
}

ContainerEditorWidget::~ContainerEditorWidget() = default;

void ContainerEditorWidget::SetModel(SampleModel* model, mvvm::SessionItem* root_item)
{
  if (!model)
  {
    return;
  }

  m_model = model;
  m_container = root_item;

  // setting up the tree
  m_view_model = std::make_unique<DragViewModel>(model);
  m_view_model->SetRootSessionItem(m_container);
  m_tree_view->setModel(m_view_model.get());
  m_tree_view->setItemDelegate(m_delegate.get());
  m_tree_view->expandAll();
  m_tree_view->header()->setSectionResizeMode(QHeaderView::Stretch);
  m_tree_view->setSelectionMode(QAbstractItemView::ContiguousSelection);

  setAcceptDrops(true);
  m_tree_view->setDragEnabled(true);
  m_tree_view->viewport()->setAcceptDrops(true);
  m_tree_view->setDropIndicatorShown(true);
}

void ContainerEditorWidget::OnAdd()
{
  m_model->AppendRandomItem(m_container);
}

void ContainerEditorWidget::OnCopy()
{
  for (auto item : GetSelectedItems())
  {
    mvvm::utils::CopyItem(item, m_model, m_container, mvvm::TagIndex::Append());
  }
}

void ContainerEditorWidget::OnRemove()
{
  for (auto item : GetSelectedItems())
  {
    m_model->RemoveItem(item);
  }
}

void ContainerEditorWidget::OnMoveDown()
{
  auto items = GetSelectedItems();
  std::reverse(items.begin(), items.end());  // to correctly move multiple selections
  for (auto item : items)
  {
    mvvm::utils::MoveDown(*item);
  }
}

void ContainerEditorWidget::OnMoveUp()
{
  for (auto item : GetSelectedItems())
  {
    mvvm::utils::MoveUp(*item);
  }
}

std::vector<mvvm::SessionItem*> ContainerEditorWidget::GetSelectedItems() const
{
  return mvvm::utils::ParentItemsFromIndex(m_tree_view->selectionModel()->selectedIndexes());
}

QBoxLayout* ContainerEditorWidget::CreateButtonLayout() const
{
  auto result = new QHBoxLayout;
  result->setContentsMargins(0, 5, 0, 5);

  auto button = new QPushButton("Add");
  button->setToolTip("Append new item at the bottom");
  connect(button, &QPushButton::clicked, this, &ContainerEditorWidget::OnAdd);
  result->addWidget(button);

  button = new QPushButton("Copy");
  button->setToolTip("Copy selected item below");
  connect(button, &QPushButton::clicked, this, &ContainerEditorWidget::OnCopy);
  result->addWidget(button);

  button = new QPushButton("Remove");
  button->setToolTip("Remove selected item");
  connect(button, &QPushButton::clicked, this, &ContainerEditorWidget::OnRemove);
  result->addWidget(button);

  button = new QPushButton("Down");
  button->setToolTip("Move selected item down");
  connect(button, &QPushButton::clicked, this, &ContainerEditorWidget::OnMoveDown);
  result->addWidget(button);

  button = new QPushButton("Up");
  button->setToolTip("Move selected item up");
  connect(button, &QPushButton::clicked, this, &ContainerEditorWidget::OnMoveUp);
  result->addWidget(button);

  return result;
}

}  // namespace dragandmove
