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

#include "mvvm/widgets/item_selection_model.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/viewmodel/viewmodel.h>
#include <mvvm/core/exceptions.h>

namespace mvvm
{

ItemSelectionModel::ItemSelectionModel(mvvm::ViewModel *view_model, QObject *parent)
    : QItemSelectionModel(nullptr, parent)
{
  connect(this, &ItemSelectionModel::selectionChanged, this,
          [this](auto, auto) { emit SelectedItemChanged(GetSelectedItem()); });

  SetViewModel(view_model);
}

void ItemSelectionModel::SetViewModel(mvvm::ViewModel *view_model)
{
  setModel(view_model);

  if (view_model)
  {
    connect(view_model, &mvvm::ViewModel::modelAboutToBeReset, this,
            &ItemSelectionModel::OnViewModelReset, Qt::UniqueConnection);
  }
}

const mvvm::SessionItem *ItemSelectionModel::GetSelectedItem() const
{
  auto selected = GetSelectedItems();
  return selected.empty() ? nullptr : selected.front();
}

std::vector<const mvvm::SessionItem *> ItemSelectionModel::GetSelectedItems() const
{
  std::vector<const mvvm::SessionItem *> result;

  for (auto index : selectedIndexes())
  {
    // skipping nullptr
    if (auto item = GetViewModel()->GetSessionItemFromIndex(index); item)
    {
      result.push_back(item);
    }
  }

  return mvvm::utils::UniqueWithOrder(result);
}

void ItemSelectionModel::SetSelectedItem(const mvvm::SessionItem *item)
{
  SetSelectedItems({item});
}

void ItemSelectionModel::SetSelectedItems(std::vector<const mvvm::SessionItem *> items)
{
  if (!GetViewModel())
  {
    throw ArgumentNullException("ViewModel is not initialised");
  }

  clearSelection();
  QItemSelection selection;
  for (auto item : items)
  {
    for (auto index : GetViewModel()->GetIndexOfSessionItem(item))
    {
      selection.push_back(QItemSelectionRange(index));
    }
  }

  //  auto flags = QItemSelectionModel::Select;  // not clear, which one to use
  auto flags = QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows;
  select(selection, flags);
}

void ItemSelectionModel::OnViewModelReset()
{
  // we've made it member function, and not lambda, because UniqueConnection doesn't work with
  // lambda
  clearSelection();
}

const mvvm::ViewModel *ItemSelectionModel::GetViewModel() const
{
  return dynamic_cast<const mvvm::ViewModel *>(model());
}

}  // namespace mvvm
