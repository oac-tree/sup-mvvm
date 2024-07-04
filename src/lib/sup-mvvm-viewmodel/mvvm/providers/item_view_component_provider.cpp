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

#include "item_selection_model.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/providers/viewmodel_delegate.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <QAbstractItemView>

namespace mvvm
{

ItemViewComponentProvider::ItemViewComponentProvider(std::unique_ptr<ViewModel> view_model,
                                                     QAbstractItemView *view)
    : m_delegate(std::make_unique<ViewModelDelegate>())
    , m_selection_model(std::make_unique<ItemSelectionModel>())
    , m_view_model(std::move(view_model))
    , m_view(view)
{
  if (!view)
  {
    throw RuntimeException("View is not initialised");
  }

  connect(m_selection_model.get(), &ItemSelectionModel::SelectedItemChanged, this,
          [this](auto item) { emit SelectedItemChanged(const_cast<SessionItem *>(item)); });

  m_selection_model->SetViewModel(m_view_model.get());
  m_view->setModel(m_view_model.get());
  m_view->setItemDelegate(m_delegate.get());
  m_view->setSelectionModel(m_selection_model.get());
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

QAbstractItemView *ItemViewComponentProvider::GetView() const
{
  return m_view;
}

QItemSelectionModel *ItemViewComponentProvider::GetSelectionModel() const
{
  return m_selection_model.get();
}

ViewModel *ItemViewComponentProvider::GetViewModel() const
{
  return m_view_model.get();
}

const SessionItem *ItemViewComponentProvider::GetItemFromViewIndex(const QModelIndex &index) const
{
  return GetViewModel()->GetSessionItemFromIndex(index);
}

QList<QModelIndex> ItemViewComponentProvider::GetViewIndices(const SessionItem *item) const
{
  return GetViewModel()->GetIndexOfSessionItem(item);
}

SessionItem *ItemViewComponentProvider::GetSelectedItem() const
{
  return const_cast<SessionItem *>(m_selection_model->GetSelectedItem());
}

void ItemViewComponentProvider::SetSelectedItem(SessionItem *item)
{
  m_selection_model->SetSelectedItem(item);
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
