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

#include "viewmodel_utils.h"

#include "presentation_item_helper.h"
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
  // In Qt when we are editing the data in a view two roles are emmited.
  if ((role == DataRole::kDisplay) || (role == DataRole::kData))
  {
    return {GetQtRole(Qt::DisplayRole), GetQtRole(Qt::EditRole)};
  }

  if (role == DataRole::kAppearance)
  {
    return {GetQtRole(Qt::ForegroundRole)};
  }

  if (role == DataRole::kTooltip)
  {
    return {GetQtRole(Qt::ToolTipRole)};
  }

  if (role == DataRole::kTextColor)
  {
    return {GetQtRole(Qt::ForegroundRole)};
  }

  if (role == DataRole::kCheckState)
  {
    return {GetQtRole(Qt::CheckStateRole)};
  }

  return {};
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

QString GetPlaceholderText(const SessionItem& item)
{
  static const std::string kDefaultPlaceholder = "default";
  return QString::fromStdString(item.HasData(DataRole::kPlaceholder)
                                    ? item.Data<std::string>(DataRole::kPlaceholder)
                                    : kDefaultPlaceholder);
}

std::int32_t GetPrecision(const SessionItem& item)
{
  return item.HasData(DataRole::kPrecision) ? item.Data<std::int32_t>(DataRole::kPrecision)
                                            : constants::kDefaultDoubleDecimals;
}

QVariant CheckStateRole(const SessionItem& item)
{
  // item has check state role explicitly defined
  if (item.HasData(DataRole::kCheckState))
  {
    // Item can carry any integer, Qt will understand only 0,1,2 for unchecked, partially checked,
    // and checked states. It is up to the item creator to set proper values.
    return item.Data<int>(DataRole::kCheckState);
  }

  // item data role carries a boolean value, which can be represented as check state role
  if (TypeName(item.Data()) == constants::kBooleanTypeName)
  {
    return item.Data<bool>() ? Qt::Checked : Qt::Unchecked;
  }

  return {};
}

QVariant DecorationRole(const SessionItem& item)
{
  if (TypeName(item.Data()) == constants::kStringTypeName
      && item.GetEditorType() == constants::kColorEditorType)
  {
    return QColor(QString::fromStdString(item.Data<std::string>()));
  }

  if (TypeName(item.Data()) == constants::kExternalPropertyTypeName)
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

QVector<std::int32_t> GetQtRolesToEmit(const ViewItem* view, role_t item_role)
{
  if (auto presentation = dynamic_cast<const AbstractPresentationItem*>(view->GetItemData());
      presentation)
  {
    return presentation->GetQtRolesToEmit(item_role);
  }

  return {};
}

}  // namespace mvvm::utils
