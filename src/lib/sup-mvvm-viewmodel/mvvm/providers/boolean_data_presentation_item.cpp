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

#include "boolean_data_presentation_item.h"

#include "presentation_item_helper.h"

#include <mvvm/model/session_item.h>
#include <mvvm/providers/custom_variants.h>

namespace mvvm
{

BooleanDataPresentationItem::BooleanDataPresentationItem(SessionItem* item,
                                                         AppearanceMode appearance_mode)
    : DefaultDataPresentationItem(item, DataRole::kData), m_appearance_mode(appearance_mode)
{
}

QVector<role_t> BooleanDataPresentationItem::GetQtRolesToEmit(role_t data_role) const
{
  if (data_role == DataRole::kData)
  {
    return {GetQtRole(Qt::DisplayRole), GetQtRole(Qt::EditRole), GetQtRole(Qt::CheckStateRole)};
  }

  return {};
}

bool BooleanDataPresentationItem::IsEditable() const
{
  // We will be able to edit the data via checkbox, so we return false here to avoid creating an
  // editor in a cell.
  return false;
}

bool BooleanDataPresentationItem::IsCheckable() const
{
  return true;
}

QVariant BooleanDataPresentationItem::Data(role_t qt_role) const
{
  auto item = GetItem();

  if (item == nullptr)
  {
    return {};
  }

  if ((m_appearance_mode == AppearanceMode::kDefault) && (qt_role == GetQtRole(Qt::DisplayRole)))
  {
    return GetDisplayRole();
  }

  const bool is_edit_role = (qt_role == GetQtRole(Qt::EditRole));
  const bool is_check_state_role = (qt_role == GetQtRole(Qt::CheckStateRole));

  if (is_edit_role || is_check_state_role)
  {
    auto value = item->Data(DataRole::kData);
    if (GetTypeCode(value) != TypeCode::Bool)
    {
      return {};
    }

    const bool is_checked = std::get<bool>(value);
    // Qt check state role is converted from main data role
    return is_edit_role ? QVariant(is_checked) : QVariant(is_checked ? Qt::Checked : Qt::Unchecked);
  }

  return {};
}

bool BooleanDataPresentationItem::SetData(const QVariant& data, role_t qt_role)
{
  if (auto item = GetItem(); item)
  {
    if (qt_role == GetQtRole(Qt::CheckStateRole))
    {
      // check state is converted to main data role
      return item->SetData(data.toInt() == Qt::Checked, DataRole::kData);
    }

    if ((qt_role == GetQtRole(Qt::EditRole)) && utils::IsBoolVariant(data))
    {
      return item->SetData(data.toBool(), DataRole::kData);
    }
  }

  return false;
}

}  // namespace mvvm
