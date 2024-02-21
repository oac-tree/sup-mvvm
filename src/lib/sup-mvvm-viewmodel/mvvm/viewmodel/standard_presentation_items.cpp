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

#include "standard_presentation_items.h"

#include "variant_converter.h"
#include "viewmodel_utils.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/sessionitem.h>

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

bool SessionItemPresentation::IsValidItemDataRole(int data_role) const
{
  return m_data_role == data_role || data_role == DataRole::kAppearance
         || data_role == DataRole::kTooltip;
}

QVector<int> SessionItemPresentation::GetQtRoles(int data_role) const
{
  return IsValidItemDataRole(data_role) ? utils::ItemRoleToQtRole(data_role) : QVector<int>();
}

// ------------------------------------------------------------------------------------------------
// LabelPresentationItem
// ------------------------------------------------------------------------------------------------

LabelPresentationItem::LabelPresentationItem(SessionItem *item, const std::string &label)
    : SessionItemPresentation(item, DataRole::kDisplay), m_label(label)
{
}

QVariant LabelPresentationItem::Data(int qt_role) const
{
  // use item's display role
  if (qt_role == Qt::DisplayRole)
  {
    return QString::fromStdString(m_label);
  }

  return SessionItemPresentation::Data(qt_role);
}

// ------------------------------------------------------------------------------------------------
// DataPresentationItem
// ------------------------------------------------------------------------------------------------

DataPresentationItem::DataPresentationItem(SessionItem *item, int role)
    : SessionItemPresentation(item, role)
{
  if (!item)
  {
    throw RuntimeException("Error in DataPresentationItem: uninitialized item");
  }
}

QVariant DataPresentationItem::Data(int qt_role) const
{
  if (qt_role == Qt::DisplayRole || qt_role == Qt::EditRole)
  {
    return GetQtVariant(GetItem()->Data(GetDataRole()));
  }

  if (qt_role == Qt::CheckStateRole)
  {
    return utils::CheckStateRole(*GetItem());
  }

  if (qt_role == Qt::DecorationRole)
  {
    return utils::DecorationRole(*GetItem());
  }

  return SessionItemPresentation::Data(qt_role);
}

bool DataPresentationItem::SetData(const QVariant &data, int qt_role)
{
  return qt_role == Qt::EditRole ? GetItem()->SetData(GetStdVariant(data), GetDataRole()) : false;
}

// ------------------------------------------------------------------------------------------------
// DisplayNamePresentationItem
// ------------------------------------------------------------------------------------------------

DisplayNamePresentationItem::DisplayNamePresentationItem(SessionItem *item)
    : SessionItemPresentation(item, DataRole::kDisplay)
{
  if (!item)
  {
    throw RuntimeException("Error in DisplayNamePresentationItem: uninitialized item");
  }
}

QVariant DisplayNamePresentationItem::Data(int qt_role) const
{
  // use item's display role
  if (qt_role == Qt::DisplayRole)
  {
    return QString::fromStdString(GetItem()->GetDisplayName());
  }

  return SessionItemPresentation::Data(qt_role);
}

// ------------------------------------------------------------------------------------------------
// EditableDisplayNamePresentationItem
// ------------------------------------------------------------------------------------------------

EditableDisplayNamePresentationItem::EditableDisplayNamePresentationItem(SessionItem *item)
    : DataPresentationItem(item, DataRole::kDisplay)
{
}

QVariant EditableDisplayNamePresentationItem::Data(int qt_role) const
{
  // use item's display role
  if (qt_role == Qt::DisplayRole || qt_role == Qt::EditRole)
  {
    // Display name is different from other roles. When display name is not present, we use item
    // type for that. This is why we are using GetDisplayName() method and not directly call
    // DataPresentationItem::Data() method.
    return QString::fromStdString(GetItem()->GetDisplayName());
  }

  if (qt_role == Qt::CheckStateRole)
  {
    return {};
  }

  return DataPresentationItem::Data(qt_role);
}

// ------------------------------------------------------------------------------------------------
// FixedDataPresentationItem
// ------------------------------------------------------------------------------------------------

FixedDataPresentationItem::FixedDataPresentationItem(SessionItem *item,
                                                     std::map<int, QVariant> fixed_data)
    : SessionItemPresentation(item, DataRole::kUser), m_data(std::move(fixed_data))
{
  for (auto &[qt_role, data] : m_data)
  {
    SetData(data, qt_role);
  }
}

QVector<int> FixedDataPresentationItem::GetQtRoles(int data_role) const
{
  QVector<int> result;
  for (auto &[qt_role, data] : m_data)
  {
    result.push_back(qt_role);
  }
  return result;
}

QVariant FixedDataPresentationItem::Data(int qt_role) const
{
  auto iter = m_data.find(qt_role);

  if (iter == m_data.end() && qt_role == Qt::ForegroundRole)
  {
    // if there is no dedicated ForegroundRole, have to use this from parent class
    // this will allow to show text in gray, if SessionItem is disabled
    return SessionItemPresentation::Data(qt_role);
  }

  return iter != m_data.end() ? iter->second : QVariant();
}

bool FixedDataPresentationItem::SetData(const QVariant &data, int qt_role)
{
  m_data[qt_role] = data;
  return true;
}

}  // namespace mvvm
