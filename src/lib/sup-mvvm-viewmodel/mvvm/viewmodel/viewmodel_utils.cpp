/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/viewmodel_utils.h"

#include <mvvm/core/variant.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <QColor>
#include <set>

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
  // disabled item is displayed by gray color
  if (item.HasFlag(Appearance::kDisabled))
  {
    return QColor(Qt::gray);
  }

  // For hidden item we select gray color too. Technical views normally shows hidden item in
  // gray, normal property editors do not show them at all.
  if (item.HasFlag(Appearance::kHidden))
  {
    return QColor(Qt::gray);
  }

  if (item.HasFlag(Appearance::kHighlighted))
  {
    return QColor(Qt::darkRed);
  }

  return QVariant();
}

QVariant CheckStateRole(const SessionItem& item)
{
  if (TypeName(item.Data()) == ::mvvm::constants::kBooleanTypeName)
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

SessionItem* ItemFromIndex(const QModelIndex& index)
{
  if (auto model = dynamic_cast<const ViewModel*>(index.model()); model)
  {
    return const_cast<SessionItem*>(model->GetSessionItemFromIndex(index));
  }

  return nullptr;
}

std::vector<SessionItem*> ItemsFromIndex(const QModelIndexList& index_list)
{
  if (index_list.empty())
  {
    return {};
  }

  std::vector<SessionItem*> result;

  if (auto model = dynamic_cast<const ViewModel*>(index_list.front().model()))
  {
    std::transform(index_list.begin(), index_list.end(), std::back_inserter(result),
                   [model](auto index)
                   { return const_cast<SessionItem*>(model->GetSessionItemFromIndex(index)); });
  }

  return result;
}

std::vector<SessionItem*> ParentItemsFromIndex(const QModelIndexList& index_list)
{
  std::set<SessionItem*> unique_parents;
  for (auto item : ItemsFromIndex(index_list))
  {
    if (item)
    {
      unique_parents.insert(item->GetParent());
    }
  }

  std::vector<SessionItem*> result;
  std::copy(unique_parents.begin(), unique_parents.end(), std::back_inserter(result));
  return result;
}

}  // namespace mvvm::utils
