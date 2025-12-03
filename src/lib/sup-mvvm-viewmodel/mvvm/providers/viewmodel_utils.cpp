/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "viewmodel_utils.h"

#include "standard_presentation_items.h"

#include <mvvm/core/variant.h>
#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <QAbstractProxyModel>
#include <QColor>
#include <set>

namespace mvvm::utils
{

namespace
{

/**
 * @brief Returns original source index from a view index, which presumably contains a chain of
 * proxy models.
 */
QModelIndex GetSourceIndexFromProxyIndex(const QModelIndex& index)
{
  QModelIndex source = index;
  while (true)
  {
    if (auto model_proxy = dynamic_cast<const QAbstractProxyModel*>(source.model()); model_proxy)
    {
      source = model_proxy->mapToSource(source);
    }
    else
    {
      break;
    }
  }

  return source;
}

}  // namespace

QVector<std::int32_t> ItemRoleToQtRole(role_t role)
{
  QVector<std::int32_t> result;
  // In Qt when we are editing the data in a view two roles are emmited.
  if ((role == DataRole::kDisplay) || (role == DataRole::kData))
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
  else
  {
  }

  return result;
}

QVariant TextColorRole(const SessionItem& item)
{
  if (item.HasData(DataRole::kTextColor))
  {
    return QColor(QString::fromStdString(item.Data<std::string>(DataRole::kTextColor)));
  }

  // disabled item is displayed by gray color
  if (item.HasFlag(Appearance::kDisabled))
  {
    return QColor(Qt::gray);
  }

  if (item.HasFlag(Appearance::kHighlighted))
  {
    return QColor(Qt::darkRed);
  }

  return {};
}

QVariant CheckStateRole(const SessionItem& item)
{
  if (TypeName(item.Data()) == ::mvvm::constants::BooleanTypeName())
  {
    return item.Data<bool>() ? Qt::Checked : Qt::Unchecked;
  }
  return {};
}

QVariant DecorationRole(const SessionItem& item)
{
  if (TypeName(item.Data()) == ::mvvm::constants::StringTypeName()
      && item.GetEditorType() == ::mvvm::constants::kColorEditorType)
  {
    return QColor(QString::fromStdString(item.Data<std::string>()));
  }

  if (TypeName(item.Data()) == ::mvvm::constants::ExternalPropertyTypeName())
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

SessionItem* ItemFromProxyIndex(const QModelIndex& index)
{
  return ItemFromIndex(GetSourceIndexFromProxyIndex(index));
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
    (void)std::transform(
        index_list.begin(), index_list.end(), std::back_inserter(result), [model](auto index)
        { return const_cast<SessionItem*>(model->GetSessionItemFromIndex(index)); });
  }

  return result;
}

std::vector<SessionItem*> ParentItemsFromIndex(const QModelIndexList& index_list)
{
  std::set<SessionItem*> unique_parents;
  for (auto item : ItemsFromIndex(index_list))
  {
    if (item != nullptr)
    {
      (void)unique_parents.insert(item->GetParent());
    }
  }

  std::vector<SessionItem*> result;
  (void)std::copy(unique_parents.begin(), unique_parents.end(), std::back_inserter(result));
  return result;
}

QVector<std::int32_t> GetQtRoles(const ViewItem* view, role_t item_role)
{
  if (auto presentation = dynamic_cast<const SessionItemPresentation*>(view->GetItemData());
      presentation)
  {
    return presentation->GetQtRoles(item_role);
  }

  return {};
}

}  // namespace mvvm::utils
