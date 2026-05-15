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

DefaultDataPresentationItem::DefaultDataPresentationItem(SessionItem* item, role_t role)
    : AbstractItemPresentation(item), m_main_data_role(role)
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
  (void)data;
  (void)qt_role;
  return false;
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

  return GetItemDataDisplayRole(*item, m_main_data_role);
}

}  // namespace mvvm
