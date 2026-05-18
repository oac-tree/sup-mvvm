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

#include <mvvm/model/session_item.h>

namespace mvvm
{

BooleanDataPresentationItem::BooleanDataPresentationItem(SessionItem* item,
                                                         BooleanAppearanceMode appearance_mode)
    : DefaultDataPresentationItem(item, DataRole::kData), m_appearance_mode(appearance_mode)
{
}

QVector<role_t> BooleanDataPresentationItem::GetQtRolesToEmit(role_t data_role) const
{
  if (data_role == DataRole::kData)
  {
    return {Qt::DisplayRole, Qt::EditRole, Qt::CheckStateRole};
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

  if ((m_appearance_mode == BooleanAppearanceMode::kDefault) && (qt_role == Qt::DisplayRole))
  {
    return GetDisplayRole();
  }

  if (qt_role == Qt::CheckStateRole)
  {
    // Qt check state role is converted from main data role
    return item->Data<bool>() ? Qt::Checked : Qt::Unchecked;
  }

  return {};
}

bool BooleanDataPresentationItem::SetData(const QVariant& data, role_t qt_role)
{
  if (auto item = GetItem(); item)
  {
    if (qt_role == Qt::CheckStateRole)
    {
      // check state is converted to main data role
      return item->SetData(data.toInt() == Qt::Checked, DataRole::kData);
    }
  }

  return false;
}

}  // namespace mvvm
