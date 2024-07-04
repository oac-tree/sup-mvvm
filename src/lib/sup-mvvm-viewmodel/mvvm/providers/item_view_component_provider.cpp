/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "item_view_component_provider.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/providers/viewmodel_delegate.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <QAbstractItemView>
#include <QAbstractProxyModel>

namespace mvvm
{

ItemViewComponentProvider::ItemViewComponentProvider(std::unique_ptr<ViewModel> view_model,
                                                     QAbstractItemView *view)
    : m_delegate(std::make_unique<ViewModelDelegate>())
    , m_view_model(std::move(view_model))
    , m_view(view)
{
  if (!view)
  {
    throw RuntimeException("View is not initialised");
  }

  m_view->setModel(m_view_model.get());
  m_view->setItemDelegate(m_delegate.get());

  connect(GetSelectionModel(), &QItemSelectionModel::selectionChanged, this,
          [this](auto, auto) { emit SelectedItemChanged(GetSelectedItem()); });

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
  return m_view->selectionModel();
}

ViewModel *ItemViewComponentProvider::GetViewModel() const
{
  return m_view_model.get();
}

const SessionItem *ItemViewComponentProvider::GetItemFromViewIndex(const QModelIndex &index) const
{
  auto source_index = index;
  auto proxies = GetProxyModelChain();
  for (auto it = proxies.rbegin(); it != proxies.rend(); ++it)
  {
    source_index = (*it)->mapToSource(index);
  }

  return GetViewModel()->GetSessionItemFromIndex(source_index);
}

QList<QModelIndex> ItemViewComponentProvider::GetViewIndices(const SessionItem *item) const
{
  auto source_indices = GetViewModel()->GetIndexOfSessionItem(item);

  for (const auto proxy : GetProxyModelChain())
  {
    QModelIndexList proxy_indexes;
    for (auto index : source_indices)
    {
      auto proxy_index = proxy->mapFromSource(index);
      if (proxy_index.isValid())
      {
        proxy_indexes.push_back(proxy_index);
      }
    }
    source_indices = proxy_indexes;
  }

  return source_indices;
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
    for (auto index : GetViewIndices(item))
    {
      selection.push_back(QItemSelectionRange(index));
    }
  }

  //  auto flags = QItemSelectionModel::Select;  // not clear, which one to use
  auto flags = QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows;
  GetSelectionModel()->select(selection, flags);
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

}  // namespace mvvm
