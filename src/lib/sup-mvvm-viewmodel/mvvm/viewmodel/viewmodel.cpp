/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/viewmodel.h"

#include <mvvm/model/sessionitem.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/viewmodel/abstract_viewmodel_controller.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

namespace mvvm
{
ViewModel::ViewModel(QObject* parent) : ViewModelBase(parent) {}

QVariant ViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
  {
    auto data = m_controller->GetHorizontalHeaderLabels();
    if (section < data.size())
    {
      return data.at(section);
    }
  }
  return {};
}

ViewModel::~ViewModel() = default;

const SessionItem* ViewModel::GetRootSessionItem() const
{
  return utils::GetItemFromView<SessionItem>(rootItem());
}

SessionItem* ViewModel::GetRootSessionItem()
{
  return const_cast<SessionItem*>(static_cast<const ViewModel*>(this)->GetRootSessionItem());
}

void ViewModel::SetRootSessionItem(SessionItem* item)
{
  if (!item)
  {
    throw std::runtime_error("Error: attemp to set nulptr as root item");
  }

  m_controller->SetRootItem(item);
}

const SessionItem* ViewModel::GetSessionItemFromIndex(const QModelIndex& index) const
{
  if (!m_controller)
  {
    throw std::runtime_error("Invalid controller");
  }
  return index.isValid() ? utils::GetItemFromView<SessionItem>(itemFromIndex(index))
                         : GetRootSessionItem();
}

ViewItem* ViewModel::GetViewItemFromIndex(const QModelIndex& index) const
{
  return itemFromIndex(index);
}

//! Returns vector of all ViewItem's representing given SessionItem.

std::vector<ViewItem*> ViewModel::FindViews(const SessionItem* item) const
{
  return mvvm::utils::FindViewsForItem<SessionItem>(this, item);
}

//! Returns list of model indices representing given SessionItem.

QModelIndexList ViewModel::GetIndexOfSessionItem(const SessionItem* item) const
{
  QModelIndexList result;
  for (auto view : FindViews(item))
  {
    auto index = indexFromItem(view);
    if (index.isValid())
    {
      result.push_back(index);
    }
  }
  return result;
}

int ViewModel::columnCount(const QModelIndex& parent) const
{
  return m_controller ? m_controller->GetHorizontalHeaderLabels().count()
                      : ViewModelBase::columnCount(parent);
}

void ViewModel::SetController(std::unique_ptr<AbstractViewModelController> controller)
{
  m_controller = std::move(controller);
  auto model = m_controller->GetModel();
  m_controller->SetRootItem(model->GetRootItem());
}

AbstractViewModelController* ViewModel::Controller()
{
  return m_controller.get();
}

}  // namespace mvvm
