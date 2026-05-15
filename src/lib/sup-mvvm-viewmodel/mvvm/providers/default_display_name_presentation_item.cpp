/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "default_display_name_presentation_item.h"

#include "presentation_item_helper.h"
#include "viewmodel_utils.h"

namespace mvvm
{

DefaultDisplayNamePresentationItem::DefaultDisplayNamePresentationItem(SessionItem* item)
    : AbstractPresentationItem(item), m_main_data_role(DataRole::kDisplay)
{
}

bool DefaultDisplayNamePresentationItem::IsEditable() const
{
  auto item = GetItem();

  if (item == nullptr)
  {
    return false;
  }

  return item->IsEditable() && item->HasFlag(Appearance::kEditableDisplayName);
}

bool DefaultDisplayNamePresentationItem::IsCheckable() const
{
  return false;
}

QVariant DefaultDisplayNamePresentationItem::Data(role_t qt_role) const
{
  auto item = GetItem();

  if (item == nullptr)
  {
    return {};
  }

  if ((qt_role == Qt::DisplayRole) || (qt_role == Qt::EditRole))
  {
    // No matter if item is editable or not, we report display name for both DisplayRole and
    // EditRole. This is because QDataWidgetMapper uses EditRole while mapping to QLabel.
    return QString::fromStdString(item->GetDisplayName());
  }

  if (qt_role == Qt::ForegroundRole)
  {
    return utils::TextColorRole(*item);
  }

  if (qt_role == Qt::ToolTipRole)
  {
    return utils::ToolTipRole(*item);
  }

  // no CheckState or Decoration for simple display name item

  return {};
}

bool DefaultDisplayNamePresentationItem::SetData(const QVariant& data, role_t qt_role)
{
  auto item = GetItem();

  if (item == nullptr)
  {
    return {};
  }

  if (IsEditable() && (qt_role == Qt::EditRole))
  {
    return item->SetData(data.toString().toStdString(), DataRole::kDisplay);
  }

  return false;
}

QVector<role_t> DefaultDisplayNamePresentationItem::GetQtRolesToEmit(role_t data_role) const
{
  // this presentation has nothing to do with mvvm::DataRole::kData
  return (data_role == DataRole::kData) ? QVector<role_t>() : utils::ItemRoleToQtRole(data_role);
}

role_t DefaultDisplayNamePresentationItem::GetMainDataRole() const
{
  return m_main_data_role;
}

}  // namespace mvvm
