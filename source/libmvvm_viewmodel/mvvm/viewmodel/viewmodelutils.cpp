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
#include "mvvm/standarditems/editor_constants.h"

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

  // Disabled item is displayed by gray color.
  // Hidden item is displayed by gray color too, it's up to the view to decide how to show it.

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

QVariant DecorationRole(const SessionItem& item)
{
  if (TypeName(item.Data()) == ::mvvm::constants::kStringTypeName
      && item.GetEditorType() == ::mvvm::constants::kColorEditorType)
  {
    return QColor(QString::fromStdString(item.Data<std::string>()));
  }

  if (TypeName(item.Data()) == ::mvvm::constants::kExternalPropertyTypeName)
  {
    return QColor(QString::fromStdString(item.Data<ExternalProperty>().GetColorName()));
  }

  return {};
}

QVariant ToolTipRole(const SessionItem& item)
{
  return item.HasData(DataRole::kTooltip) ? QVariant(QString::fromStdString(item.GetToolTip()))
                                          : QVariant();
}

}  // namespace mvvm::utils
