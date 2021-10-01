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

#include "mvvm/viewmodel/viewmodel.h"

#include "mvvm/viewmodel/abstractviewmodelcontroller.h"
#include "mvvm/viewmodelbase/viewmodelbaseutils.h"

namespace ModelView
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
  return QVariant();
}

ViewModel::~ViewModel() = default;

const SessionItem* ViewModel::GetRootSessionItem() const
{
  return Utils::GetContext<SessionItem>(rootItem());
}

const SessionItem* ViewModel::GetSessionItemFromIndex(const QModelIndex& index) const
{
  if (!m_controller)
  {
    throw std::runtime_error("Invalid controller");
  }
  return index.isValid() ? Utils::GetContext<SessionItem>(itemFromIndex(index))
                         : GetRootSessionItem();
}

ViewItem* ViewModel::GetViewItemFromIndex(const QModelIndex& index) const
{
  return itemFromIndex(index);
}

//! Returns vector of all ViewItem's representing given SessionItem.

std::vector<ViewItem*> ViewModel::FindViews(const SessionItem* item) const
{
  return ModelView::Utils::FindViews<SessionItem>(this, item);
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

void ViewModel::SetController(std::unique_ptr<AbstractViewModelController> controller)
{
  m_controller = std::move(controller);
  m_controller->Init();
}

AbstractViewModelController* ViewModel::Controller()
{
  return m_controller.get();
}

}  // namespace ModelView
