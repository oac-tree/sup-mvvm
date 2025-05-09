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

#include "viewmodel.h"

#include "abstract_viewmodel_controller.h"
#include "viewmodel_utils.h"

#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>

namespace mvvm
{
ViewModel::ViewModel(QObject* parent_object) : ViewModelBase(parent_object) {}

ViewModel::~ViewModel() = default;

QVariant ViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  ValidateController();

  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
  {
    auto data = m_controller->GetHorizontalHeaderLabels();
    if (section >= 0 && section < data.size())
    {
      return data.at(section);
    }
  }
  return {};
}

int ViewModel::columnCount(const QModelIndex& parent) const
{
  return m_controller ? m_controller->GetColumnCount() : ViewModelBase::columnCount(parent);
}

bool ViewModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!index.isValid())
  {
    return false;
  }

  if (auto item = itemFromIndex(index); item)
  {
    // Please note the following difference with ViewModelBase::setData. Here we do not have
    // explicit notification. ViewModelController will do it for us after the change on SessionModel
    // side.
    return item->SetData(value, role);
  }

  return false;
}

const ISessionModel* ViewModel::GetModel() const
{
  return GetRootSessionItem() ? GetRootSessionItem()->GetModel() : nullptr;
}

void ViewModel::SetModel(ISessionModel* model)
{
  ValidateController();
  m_controller->SetModel(model);
}

const SessionItem* ViewModel::GetRootSessionItem() const
{
  return m_controller ? m_controller->GetRootItem() : nullptr;
}

SessionItem* ViewModel::GetRootSessionItem()
{
  return const_cast<SessionItem*>(std::as_const(*this).GetRootSessionItem());
}

void ViewModel::SetRootSessionItem(SessionItem* item)
{
  m_controller->SetRootItem(item);
}

const SessionItem* ViewModel::GetSessionItemFromIndex(const QModelIndex& index) const
{
  ValidateController();
  return index.isValid() ? utils::GetItemFromView<SessionItem>(itemFromIndex(index))
                         : GetRootSessionItem();
}

SessionItem* ViewModel::GetSessionItemFromIndex(const QModelIndex& index)
{
  return const_cast<SessionItem*>(std::as_const(*this).GetSessionItemFromIndex(index));
}

const ViewItem* ViewModel::GetViewItemFromIndex(const QModelIndex& index) const
{
  return itemFromIndex(index);
}

ViewItem* ViewModel::GetViewItemFromIndex(const QModelIndex& index)
{
  return const_cast<ViewItem*>(std::as_const(*this).GetViewItemFromIndex(index));
}

std::vector<const ViewItem*> ViewModel::FindViews(const SessionItem* item) const
{
  return mvvm::utils::FindViewsForItem<SessionItem>(this, item);
}

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

void ViewModel::SetController(std::unique_ptr<AbstractViewModelController> controller)
{
  m_controller = std::move(controller);
}

AbstractViewModelController* ViewModel::Controller()
{
  return m_controller.get();
}

void ViewModel::ValidateController() const
{
  if (!m_controller)
  {
    throw RuntimeException("ViewModel: invalid controller");
  }
}

}  // namespace mvvm
