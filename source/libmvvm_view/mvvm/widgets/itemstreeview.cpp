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

#include "mvvm/model/itemutils.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/viewmodel.h"
#include "mvvm/delegates/viewmodeldelegate.h"

#include <QTreeView>
#include <QVBoxLayout>

namespace mvvm
{
ItemsTreeView::ItemsTreeView(QWidget* parent)
    : QWidget(parent)
    , m_tree_view(new QTreeView)
    , m_delegate(std::make_unique<ViewModelDelegate>())
    , m_block_selection(false)
{
  auto layout = new QVBoxLayout(this);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_tree_view);
}

ItemsTreeView::~ItemsTreeView() = default;

void ItemsTreeView::Reset()
{
  m_tree_view->setModel(nullptr);
  m_view_model.reset();
}

void ItemsTreeView::SetViewModel(std::unique_ptr<ViewModel> view_model)
{
  m_view_model = std::move(view_model);
  m_tree_view->setModel(m_view_model.get());
  m_tree_view->setItemDelegate(m_delegate.get());
  m_tree_view->expandAll();
  m_tree_view->resizeColumnToContents(0);
  SetConnected(true);
}

//! Make given item selected in QTreeView.

void ItemsTreeView::SetSelected(SessionItem* item)
{
  SetSelectedItems({item});
}

void ItemsTreeView::SetSelectedItems(const std::vector<SessionItem*>& to_select)
{
  if (!m_view_model)
  {
    return;
  }

  GetSelectionModel()->clearSelection();
  for (auto item : to_select)
  {
    for (auto index : m_view_model->GetIndexOfSessionItem(item))
    {
      GetSelectionModel()->select(index, QItemSelectionModel::Select);
    }
  }
}

void ItemsTreeView::SetRootSessionItem(SessionItem* item)
{
  if (!m_view_model)
  {
    throw std::runtime_error("Error in ItemsTreeView: ViewModel is not defined");
  }
  m_view_model->SetRootSessionItem(item);
  m_tree_view->expandAll();
}

ViewModel* ItemsTreeView::GetViewModel() const
{
  return m_view_model.get();
}

SessionItem* ItemsTreeView::GetSelectedItem() const
{
  auto items = GetSelectedItems();
  return items.empty() ? nullptr : items.front();
}

std::vector<SessionItem*> ItemsTreeView::GetSelectedItems() const
{
  if (!m_tree_view->selectionModel())
  {
    return {};
  }
  std::vector<SessionItem*> result;
  for (auto index : m_tree_view->selectionModel()->selectedIndexes())
  {
    result.push_back(const_cast<SessionItem*>(m_view_model->GetSessionItemFromIndex(index)));
  }
  return utils::UniqueItems(result);
}

//! Processes selections in QTreeView. Finds SessionItem corresponding to selected indexes
//! and emit itemSelected signal.

void ItemsTreeView::onSelectionChanged(const QItemSelection&, const QItemSelection&)
{
  if (m_block_selection)
  {
    return;
  }

  for (auto item : GetSelectedItems())
  {
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
  return m_tree_view;
}

QItemSelectionModel* ItemsTreeView::GetSelectionModel()
{
  return m_tree_view->selectionModel();
}

}  // namespace mvvm
