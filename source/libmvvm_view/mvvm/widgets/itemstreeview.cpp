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

#include "mvvm/widgets/itemstreeview.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/viewmodel.h"

#include <QTreeView>
#include <QVBoxLayout>

namespace ModelView
{
ItemsTreeView::ItemsTreeView(QWidget* parent)
    : QWidget(parent), m_treeView(new QTreeView), m_block_selection(false)
{
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_treeView);
}

ItemsTreeView::~ItemsTreeView() = default;

void ItemsTreeView::SetViewModel(std::unique_ptr<ViewModel> viewModel)
{
  m_viewModel = std::move(viewModel);
  m_treeView->setModel(m_viewModel.get());
  m_treeView->expandAll();
  m_treeView->resizeColumnToContents(0);
  SetConnected(true);
}

//! Make given item selected in QTreeView.

void ItemsTreeView::SetSelected(SessionItem* item)
{
  // Provide possibility to clear selection when item == nullptr. Provide unit tests.
  // Make sure it works when SessionModel is already destroyed.
  if (!m_viewModel || !item)
  {
    return;
  }

  auto indexes = m_viewModel->GetIndexOfSessionItem(item);
  if (!indexes.empty())
  {
    GetSelectionModel()->select(indexes.at(0), QItemSelectionModel::SelectCurrent);
  }
}

void ItemsTreeView::SetRootSessionItem(SessionItem* item)
{
  m_viewModel->SetRootSessionItem(item);
  m_treeView->expandAll();
}

ViewModel* ItemsTreeView::GetViewModel() const
{
  return m_viewModel.get();
}

//! Processes selections in QTreeView. Finds SessionItem corresponding to selected indexes
//! and emit itemSelected signal.

void ItemsTreeView::onSelectionChanged(const QItemSelection&, const QItemSelection&)
{
  if (m_block_selection)
  {
    return;
  }

  auto indexes = m_treeView->selectionModel()->selectedIndexes();
  if (!indexes.empty())
  {
    auto item = m_viewModel->GetSessionItemFromIndex(indexes.at(0));
    m_block_selection = true;
    emit itemSelected(item);
    m_block_selection = false;
  }
}

void ItemsTreeView::SetConnected(bool flag)
{
  if (flag)
  {
    connect(GetSelectionModel(), &QItemSelectionModel::selectionChanged, this,
            &ItemsTreeView::onSelectionChanged);
  }
  else
  {
    disconnect(GetSelectionModel(), &QItemSelectionModel::selectionChanged, this,
               &ItemsTreeView::onSelectionChanged);
  }
}

QTreeView* ItemsTreeView::GetTreeView()
{
  return m_treeView;
}

QItemSelectionModel* ItemsTreeView::GetSelectionModel()
{
  return m_treeView->selectionModel();
}

}  // namespace ModelView
