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

#include "mvvm/viewmodel/viewmodelutils.h"

#include "mvvm/core/variant.h"
#include "mvvm/model/sessionitem.h"

#include <QColor>

namespace mvvm::utils
{
QVector<int> ItemRoleToQtRole(int role)
{
  QVector<int> result;
  // In Qt when we are editing the data in a view two roles are emmited.
  if (role == DataRole::kDisplay || role == DataRole::kData)
  {
    result = {Qt::DisplayRole, Qt::EditRole};
  }
  else if (role == DataRole::kAppearance)
  {
    result = {Qt::ForegroundRole};
  }
  else if (role == DataRole::kTooltip)
  {
    result = {Qt::ToolTipRole};
  }

  return result;
}

QVariant TextColorRole(const SessionItem& item)
{
  const bool item_hidden = !item.IsVisible();
  const bool item_disabled = !item.IsEnabled();
  return item_disabled || item_hidden ? QColor(Qt::gray) : QVariant();
}

QVariant CheckStateRole(const SessionItem& item)
{
  if (TypeName(item.Data()) == ::mvvm::constants::kBoolTypeName)
  {
    return item.Data<bool>() ? Qt::Checked : Qt::Unchecked;
  }
  return {};
}

// FIXME when EditorRole is restored

// QVariant DecorationRole(const SessionItem& item)
//{
//   auto value = item.data<QVariant>();
//   if (Utils::IsColorVariant(value))
//     return value;

//  else if (Utils::IsExtPropertyVariant(value))
//    return value.value<ExternalProperty>().color();

//  return QVariant();
//}

QVariant ToolTipRole(const SessionItem& item)
{
  return item.HasData(DataRole::kTooltip) ? QVariant(QString::fromStdString(item.GetToolTip()))
                                          : QVariant();
}

}  // namespace mvvm::utils
