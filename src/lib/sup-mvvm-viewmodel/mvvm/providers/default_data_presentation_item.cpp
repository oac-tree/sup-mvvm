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

#include "default_data_presentation_item.h"

#include "presentation_item_helper.h"
#include "variant_converter.h"
#include "viewmodel_utils.h"

namespace mvvm
{

DefaultDataPresentationItem::DefaultDataPresentationItem(SessionItem* item, role_t main_data_role)
    : AbstractPresentationItem(item), m_main_data_role(main_data_role)
{
}

bool DefaultDataPresentationItem::IsCheckable() const
{
  return false;
}

QVariant DefaultDataPresentationItem::Data(role_t qt_role) const
{
  auto item = GetItem();

  if (item == nullptr)
  {
    return {};
  }

  if (qt_role == Qt::DisplayRole)
  {
    return GetDisplayRole();
  }

  if (qt_role == Qt::EditRole)
  {
    return GetQtVariant(GetItem()->Data(GetMainDataRole()));
  }

  if (qt_role == Qt::ForegroundRole)
  {
    return utils::TextColorRole(*item);
  }

  if (qt_role == Qt::ToolTipRole)
  {
    return utils::ToolTipRole(*item);
  }

  if (qt_role == Qt::CheckStateRole)
  {
    return utils::CheckStateRole(*GetItem());
  }

  if (qt_role == Qt::DecorationRole)
  {
    return utils::DecorationRole(*GetItem());
  }

  return {};
}

bool DefaultDataPresentationItem::SetData(const QVariant& data, role_t qt_role)
{
  // We allow to set via model only main data role (which corresponds to Qt::DataRole), and check
  // state.

  if (auto item = GetItem(); item)
  {
    if (qt_role == Qt::EditRole)
    {
      return item->SetData(GetStdVariant(data), GetMainDataRole());
    }

    if (qt_role == Qt::CheckStateRole)
    {
      return item->SetData(data.toInt(), DataRole::kCheckState);
    }
  }

  return false;
}

QVector<role_t> DefaultDataPresentationItem::GetQtRolesToEmit(role_t data_role) const
{
  // this presentation has nothing to do with mvvm::DataRole::kDisplay
  return (data_role == DataRole::kDisplay) ? QVector<role_t>() : utils::ItemRoleToQtRole(data_role);
}

role_t DefaultDataPresentationItem::GetMainDataRole() const
{
  return m_main_data_role;
}

QString DefaultDataPresentationItem::GetDisplayRole() const
{
  auto item = GetItem();

  if (item == nullptr)
  {
    return {};
  }

  // our own conversion to QString
  return GetItemDataDisplayRole(*item, m_main_data_role);
}

}  // namespace mvvm
