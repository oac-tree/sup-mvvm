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

#include "mvvm/viewmodel/standardpresentationitems.h"

#include "mvvm/model/sessionitem.h"
#include "mvvm/viewmodel/variantconverter.h"
#include "mvvm/viewmodel/viewmodelutils.h"

#include <stdexcept>

namespace mvvm
{

SessionItemPresentation::SessionItemPresentation(SessionItem *item, int role)
    : m_item(item), m_data_role(role)
{
}

QVariant SessionItemPresentation::Data(int qt_role) const
{
  if (!GetItem())
  {
    return {};
  }

  if (qt_role == Qt::ForegroundRole)
  {
    return utils::TextColorRole(*GetItem());
  }

  if (qt_role == Qt::ToolTipRole)
  {
    return utils::ToolTipRole(*GetItem());
  }

  return {};
}

bool SessionItemPresentation::SetData(const QVariant &data, int qt_role)
{
  (void)data;
  (void)qt_role;
  return false;
}

bool SessionItemPresentation::IsEnabled() const
{
  return GetItem()->IsEnabled();
}

bool SessionItemPresentation::IsEditable() const
{
  return GetItem()->IsEditable();
}

SessionItem *SessionItemPresentation::GetItem()
{
  return m_item;
}

const SessionItem *SessionItemPresentation::GetItem() const
{
  return m_item;
}

int SessionItemPresentation::GetDataRole() const
{
  return m_data_role;
}

// ----------------------------------------------------------------------------
// DataPresentationItem
// ----------------------------------------------------------------------------

DataPresentationItem::DataPresentationItem(SessionItem *item)
    : SessionItemPresentation(item, DataRole::kData)
{
  if (!item)
  {
    throw std::runtime_error("Error in DataPresentationItem: uninitialized item");
  }
}

QVariant DataPresentationItem::Data(int qt_role) const
{
  if (qt_role == Qt::DisplayRole || qt_role == Qt::EditRole)
  {
    return GetQtVariant(GetItem()->Data(DataRole::kData));
  }

  if (qt_role == Qt::CheckStateRole)
  {
    return utils::CheckStateRole(*GetItem());
  }

  // FIXME uncomment after utils::DecorationRole implementation
  //  if (role == Qt::DecorationRole)
  //      return Utils::DecorationRole(*item());

  return SessionItemPresentation::Data(qt_role);
}

bool DataPresentationItem::SetData(const QVariant &data, int qt_role)
{
  return qt_role == Qt::EditRole ? GetItem()->SetData(GetStdVariant(data), DataRole::kData) : false;
}

// ----------------------------------------------------------------------------
// LabelPresentationItem
// ----------------------------------------------------------------------------

DisplayNamePresentationItem::DisplayNamePresentationItem(SessionItem *item)
    : SessionItemPresentation(item, DataRole::kDisplay)
{
  if (!item)
  {
    throw std::runtime_error("Error in DataPresentationItem: uninitialized item");
  }
}

QVariant DisplayNamePresentationItem::Data(int qt_role) const
{
  // use item's display role
  if (qt_role == Qt::DisplayRole || qt_role == Qt::EditRole)
  {
    return QString::fromStdString(GetItem()->GetDisplayName());
  }

  return SessionItemPresentation::Data(qt_role);
}

}  // namespace mvvm
