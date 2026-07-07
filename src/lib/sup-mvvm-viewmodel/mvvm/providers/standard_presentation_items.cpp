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

#include "standard_presentation_items.h"

#include "presentation_item_helper.h"
#include "viewmodel_utils.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>

namespace mvvm
{

// ------------------------------------------------------------------------------------------------
// LabelPresentationItem
// ------------------------------------------------------------------------------------------------

LabelPresentationItem::LabelPresentationItem(SessionItem* item, const std::string& label)
    : AbstractPresentationItem(item), m_label(label)
{
}

bool LabelPresentationItem::SetData(const QVariant& data, role_t qt_role)
{
  (void)data;
  (void)qt_role;
  return false;
}

QVariant LabelPresentationItem::Data(role_t qt_role) const
{
  // use item's display role
  if (qt_role == GetQtRole(Qt::DisplayRole))
  {
    return QString::fromStdString(m_label);
  }

  if (GetItem() == nullptr)
  {
    return {};
  }

  if (qt_role == GetQtRole(Qt::ForegroundRole))
  {
    return utils::TextColorRole(*GetItem());
  }

  if (qt_role == GetQtRole(Qt::ToolTipRole))
  {
    return utils::ToolTipRole(*GetItem());
  }

  return {};
}

bool LabelPresentationItem::IsEditable() const
{
  return false;
}

QVector<role_t> LabelPresentationItem::GetQtRolesToEmit(role_t data_role) const
{
  (void)data_role;
  // LabelPresentationItem contains a fixed label which doesn't depend on SessionItem's data role.
  // We return no Qt roles here, which means that we do not want to have any notification on
  // whatever is changed on SessionItem.
  return {};
}

// ------------------------------------------------------------------------------------------------
// FixedDataPresentationItem
// ------------------------------------------------------------------------------------------------

FixedDataPresentationItem::FixedDataPresentationItem(SessionItem* item,
                                                     std::map<role_t, QVariant> fixed_data)
    : AbstractPresentationItem(item), m_data(std::move(fixed_data))
{
}

QVector<role_t> FixedDataPresentationItem::GetQtRolesToEmit(role_t data_role) const
{
  (void)data_role;
  // FixedDataPresentationItem contains a fixed data doesn't depend on SessionItem's data role.
  // We return no Qt roles here, which means that we do not want to have any notification on
  // whatever is changed on SessionItem.
  return {};
}

bool FixedDataPresentationItem::IsEditable() const
{
  return false;
}

QVariant FixedDataPresentationItem::Data(role_t qt_role) const
{
  auto iter = m_data.find(qt_role);

  if ((iter == m_data.end()) && (qt_role == GetQtRole(Qt::ForegroundRole)))
  {
    // if there is no dedicated ForegroundRole, have to use this from parent class
    // this will allow to show text in gray, if SessionItem is disabled
    return utils::TextColorRole(*GetItem());
  }

  return iter != m_data.end() ? iter->second : QVariant();
}

bool FixedDataPresentationItem::SetData(const QVariant& data, role_t qt_role)
{
  m_data[qt_role] = data;
  return true;
}

}  // namespace mvvm
