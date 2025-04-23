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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "item_view_component_provider.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/views/viewmodel_delegate.h>

#include <QAbstractItemView>
#include <QAbstractProxyModel>
#include <QItemSelectionModel>

namespace mvvm
{

ItemViewComponentProvider::ItemViewComponentProvider(std::unique_ptr<ViewModel> view_model,
                                                     QAbstractItemView *view)
    : ItemViewComponentProvider(std::make_unique<ViewModelDelegate>(), std::move(view_model), view)
{
}

ItemViewComponentProvider::ItemViewComponentProvider(
    std::unique_ptr<ViewModelDelegate> view_model_delegate, std::unique_ptr<ViewModel> view_model,
    QAbstractItemView *view)
    : m_delegate(std::move(view_model_delegate))
    , m_view_model(std::move(view_model))
    , m_view(view)
    , m_selection_model(new QItemSelectionModel(m_view_model.get(), this))
    , m_selection_flags(QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows)
{
  if (!view)
  {
    throw RuntimeException("View is not initialised");
  }

  m_view->setModel(m_view_model.get());
  m_view->setItemDelegate(m_delegate.get());
  m_view->setSelectionModel(m_selection_model);

  connect(m_selection_model, &QItemSelectionModel::selectionChanged, this,
          &ItemViewComponentProvider::OnSelectionChanged, Qt::UniqueConnection);

  connect(m_view_model.get(), &mvvm::ViewModel::modelAboutToBeReset, this,
          &ItemViewComponentProvider::OnViewModelReset, Qt::UniqueConnection);
}

ItemViewComponentProvider::~ItemViewComponentProvider() = default;

void ItemViewComponentProvider::SetApplicationModel(ISessionModel *model)
{
  m_view_model->SetModel(model);
}

void ItemViewComponentProvider::SetItem(SessionItem *item)
{
  m_view_model->SetRootSessionItem(item);
}

const SessionItem *ItemViewComponentProvider::GetItem() const
{
  return m_view_model->GetRootSessionItem();
}

SessionItem *ItemViewComponentProvider::GetItem()
{
  return m_view_model->GetRootSessionItem();
}

void ItemViewComponentProvider::AddProxyModel(std::unique_ptr<QAbstractProxyModel> proxy)
{
  if (auto last_proxy = GetLastProxyModel(); last_proxy)
  {
    proxy->setSourceModel(last_proxy);
  }
  else
  {
    proxy->setSourceModel(GetViewModel());
  }

  m_view->setModel(proxy.get());
  m_selection_model->setModel(proxy.get());

  // setModel rewrites previous selection model, need to set it again
  m_view->setSelectionModel(m_selection_model);

  m_proxy_chain.push_back(std::move(proxy));
}

QAbstractProxyModel *ItemViewComponentProvider::GetLastProxyModel() const
{
  return m_proxy_chain.empty() ? nullptr : m_proxy_chain.back().get();
}

std::vector<QAbstractProxyModel *> ItemViewComponentProvider::GetProxyModelChain() const
{
  std::vector<QAbstractProxyModel *> result;
  std::transform(m_proxy_chain.begin(), m_proxy_chain.end(), std::back_inserter(result),
                 [](const auto &child_uptr) { return child_uptr.get(); });
  return result;
}

QAbstractItemView *ItemViewComponentProvider::GetView() const
{
  return m_view;
}

QItemSelectionModel *ItemViewComponentProvider::GetSelectionModel() const
{
  return m_selection_model;
}

ViewModel *ItemViewComponentProvider::GetViewModel() const
{
  return m_view_model.get();
}

const SessionItem *ItemViewComponentProvider::GetItemFromViewIndex(const QModelIndex &index) const
{
  auto source_index = index;
  auto proxies = GetProxyModelChain();
  // The first element in proxy array is looking to the original viewmodel. The last element in
  // proxy array is what the view looks at. Iterating in reverse order.
  for (auto it = proxies.rbegin(); it != proxies.rend(); ++it)
  {
    source_index = (*it)->mapToSource(source_index);
  }

  return GetViewModel()->GetSessionItemFromIndex(source_index);
}

QList<QModelIndex> ItemViewComponentProvider::GetViewIndexes(const SessionItem *item) const
{
  auto source_indexes = GetViewModel()->GetIndexOfSessionItem(item);

  for (const auto proxy : GetProxyModelChain())
  {
    QModelIndexList proxy_indexes;
    for (auto index : source_indexes)
    {
      auto proxy_index = proxy->mapFromSource(index);
      if (proxy_index.isValid())
      {
        proxy_indexes.push_back(proxy_index);
      }
    }
    source_indexes = proxy_indexes;
  }

  return source_indexes;
}

SessionItem *ItemViewComponentProvider::GetSelectedItem() const
{
  auto selected = GetSelectedItems();
  return selected.empty() ? nullptr : selected.front();
}

void ItemViewComponentProvider::SetSelectedItem(SessionItem *item)
{
  SetSelectedItems({item});
}

void ItemViewComponentProvider::SetSelectedItems(std::vector<SessionItem *> items)
{
  GetSelectionModel()->clearSelection();
  QItemSelection selection;
  for (auto item : items)
  {
    auto view_indexes = GetViewIndexes(item);
    if (!view_indexes.empty())
    {
      selection.push_back(QItemSelectionRange(view_indexes.front(), view_indexes.back()));
    }
  }

  GetSelectionModel()->select(selection, m_selection_flags);
}

void ItemViewComponentProvider::SetSelectionFlags(QItemSelectionModel::SelectionFlags flags)
{
  m_selection_flags = flags;
}

void ItemViewComponentProvider::OnViewModelReset()
{
  GetSelectionModel()->clearSelection();
}

std::vector<SessionItem *> ItemViewComponentProvider::GetSelectedItemsIntern() const
{
  std::vector<mvvm::SessionItem *> result;

  for (auto index : GetSelectionModel()->selectedIndexes())
  {
    // skipping nullptr
    if (auto item = GetItemFromViewIndex(index); item)
    {
      result.push_back(const_cast<SessionItem *>(item));
    }
  }

  return mvvm::utils::UniqueWithOrder(result);
}

void ItemViewComponentProvider::OnSelectionChanged(const QItemSelection &selected,
                                                   const QItemSelection &deselected)
{
  Q_UNUSED(selected);
  Q_UNUSED(deselected);
  emit SelectedItemChanged(GetSelectedItem());
}

}  // namespace mvvm
