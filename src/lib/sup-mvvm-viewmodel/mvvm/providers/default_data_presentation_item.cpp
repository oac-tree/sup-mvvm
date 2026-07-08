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
  auto item = GetItem();

  if (item == nullptr)
  {
    return false;
  }

  return item->HasData(DataRole::kCheckState);
}

QVariant DefaultDataPresentationItem::Data(role_t qt_role) const
{
  auto item = GetItem();

  if (item == nullptr)
  {
    return {};
  }

  if (qt_role == GetQtRole(Qt::DisplayRole))
  {
    return GetDisplayRole();
  }

  if (qt_role == GetQtRole(Qt::EditRole))
  {
    return GetQtVariant(GetItem()->Data(GetMainDataRole()));
  }

  // Presentation-only Qt roles set on board take precedence over item-derived values (foreground,
  // decoration), and cover roles with no SessionItem equivalent at all (background).
  if (auto it = m_qt_data.find(qt_role); it != m_qt_data.end())
  {
    return it.value();
  }

  if (qt_role == GetQtRole(Qt::ForegroundRole))
  {
    return utils::TextColorRole(*item);
  }

  if (qt_role == GetQtRole(Qt::ToolTipRole))
  {
    return utils::ToolTipRole(*item);
  }

  if (qt_role == GetQtRole(Qt::CheckStateRole))
  {
    return utils::CheckStateRole(*item);
  }

  if (qt_role == GetQtRole(Qt::DecorationRole))
  {
    return utils::DecorationRole(*item);
  }

  return {};
}

bool DefaultDataPresentationItem::SetData(const QVariant& data, role_t qt_role)
{
  // We propagate to the underlying SessionItem only the main data role (which corresponds to
  // Qt::EditRole) and the check state. Presentation-only Qt roles (background, foreground) are kept
  // on board and never reach the SessionItem.

  if (auto item = GetItem(); item)
  {
    if (qt_role == GetQtRole(Qt::EditRole))
    {
      return item->SetData(GetStdVariant(data), GetMainDataRole());
    }

    if (qt_role == GetQtRole(Qt::CheckStateRole))
    {
      return item->SetData(data.toInt(), DataRole::kCheckState);
    }

    if (IsPresentationOnlyRole(qt_role))
    {
      // An invalid variant removes the entry, allowing to reset the role to its default.
      if (data.isValid())
      {
        m_qt_data[qt_role] = data;
      }
      else
      {
        (void)m_qt_data.remove(qt_role);
      }
      return true;
    }
  }

  return false;
}

bool DefaultDataPresentationItem::IsPresentationOnlyRole(role_t qt_role) const
{
  return qt_role == GetQtRole(Qt::BackgroundRole) || qt_role == GetQtRole(Qt::ForegroundRole)
         || qt_role == GetQtRole(Qt::DecorationRole);
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

QVariant DefaultDataPresentationItem::GetDisplayRole() const
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
