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

#include "viewmodel_base.h"

#include "viewitem.h"

#include <mvvm/core/mvvm_exceptions.h>

namespace mvvm
{
struct ViewModelBase::ViewModelBaseImpl
{
  ViewModelBase* model{nullptr};
  std::unique_ptr<ViewItem> root;
  explicit ViewModelBaseImpl(ViewModelBase* model) : model(model) {}

  bool IsItemBelongsToModel(ViewItem* item) const
  {
    return model->indexFromItem(item).isValid() || item == model->rootItem();
  }
};

ViewModelBase::ViewModelBase(QObject* parent_object)
    : QAbstractItemModel(parent_object), p_impl(std::make_unique<ViewModelBaseImpl>(this))
{
  beginResetModel();
  p_impl->root = std::make_unique<ViewItem>();
  endResetModel();
}

ViewModelBase::~ViewModelBase() = default;

QModelIndex ViewModelBase::index(int row, int column, const QModelIndex& parent) const
{
  auto parent_item = itemFromIndex(parent) ? itemFromIndex(parent) : rootItem();
  const bool is_valid_row = row >= 0 && row < rowCount(parent);
  const bool is_valid_column = column >= 0 && column < columnCount(parent);
  return is_valid_row && is_valid_column
             ? createIndex(row, column, parent_item->GetChild(row, column))
             : QModelIndex();
}

QModelIndex ViewModelBase::parent(const QModelIndex& child) const
{
  if (auto child_item = itemFromIndex(child); child_item)
  {
    auto parent_item = child_item->GetParent();
    return parent_item == rootItem()
               ? QModelIndex()
               : createIndex(parent_item->Row(), parent_item->Column(), parent_item);
  }

  return {};
}

int ViewModelBase::rowCount(const QModelIndex& parent) const
{
  auto parent_item = itemFromIndex(parent);
  return parent_item ? parent_item->GetRowCount() : rootItem()->GetRowCount();
}

int ViewModelBase::columnCount(const QModelIndex& parent) const
{
  auto parent_item = itemFromIndex(parent);
  return parent_item ? parent_item->GetColumnCount() : rootItem()->GetColumnCount();
}

QVariant ViewModelBase::data(const QModelIndex& index, int role) const
{
  if (!rootItem())
  {
    return {};
  }

  auto item = itemFromIndex(index);
  return item ? item->Data(role) : QVariant();
}

bool ViewModelBase::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!index.isValid())
  {
    return false;
  }

  if (auto item = itemFromIndex(index); item)
  {
    const bool result = item->SetData(value, role);
    if (result)
    {
      emit dataChanged(index, index, QVector<int>() << role);
    }
    return result;
  }

  return false;
}

const ViewItem* ViewModelBase::rootItem() const
{
  return p_impl->root.get();
}

ViewItem* ViewModelBase::rootItem()
{
  return const_cast<ViewItem*>(std::as_const(*this).rootItem());
}

ViewItem* ViewModelBase::itemFromIndex(const QModelIndex& index)
{
  return const_cast<ViewItem*>(std::as_const(*this).itemFromIndex(index));
}

const ViewItem* ViewModelBase::itemFromIndex(const QModelIndex& index) const
{
  return index.isValid() ? static_cast<const ViewItem*>(index.internalPointer()) : nullptr;
}

QModelIndex ViewModelBase::indexFromItem(const ViewItem* item) const
{
  return item && item->GetParent()
             ? createIndex(item->Row(), item->Column(), const_cast<ViewItem*>(item))
             : QModelIndex();
}

void ViewModelBase::removeRow(ViewItem* parent, int row)
{
  if (!p_impl->IsItemBelongsToModel(parent))
  {
    throw RuntimeException("Error in ViewModelBase: attempt to use parent from another model");
  }

  beginRemoveRows(indexFromItem(parent), row, row);
  parent->RemoveRow(row);
  endRemoveRows();
}

void ViewModelBase::clearRows(ViewItem* parent)
{
  if (!p_impl->IsItemBelongsToModel(parent))
  {
    throw RuntimeException("Error in ViewModelBase: attempt to use parent from another model");
  }

  if (!parent->GetRowCount())
  {
    return;
  }

  beginRemoveRows(indexFromItem(parent), 0, parent->GetRowCount() - 1);
  parent->Clear();
  endRemoveRows();
}

void ViewModelBase::insertRow(ViewItem* parent, int row,
                              std::vector<std::unique_ptr<ViewItem>> items)
{
  if (!p_impl->IsItemBelongsToModel(parent))
  {
    throw RuntimeException("Error in ViewModelBase: attempt to use parent from another model");
  }

  beginInsertRows(indexFromItem(parent), row, row);
  parent->InsertRow(row, std::move(items));
  endInsertRows();
}

void ViewModelBase::appendRow(ViewItem* parent, std::vector<std::unique_ptr<ViewItem>> items)
{
  insertRow(parent, parent->GetRowCount(), std::move(items));
}

Qt::ItemFlags ViewModelBase::flags(const QModelIndex& index) const
{
  Qt::ItemFlags result = QAbstractItemModel::flags(index);
  if (auto item = itemFromIndex(index); item)
  {
    result |= item->Flags();
  }
  return result;
}

bool ViewModelBase::hasChildren(const QModelIndex& parent) const
{
  auto parent_item = itemFromIndex(parent);
  return parent_item ? parent_item->HasChildren() : rootItem()->HasChildren();
}

void ViewModelBase::ResetRootViewItem(std::unique_ptr<ViewItem> root_item, bool notify)
{
  if (notify)
  {
    beginResetModel();
  }
  p_impl->root = std::move(root_item);
  if (notify)
  {
    endResetModel();
  }
}

void ViewModelBase::BeginResetModelNotify()
{
  beginResetModel();
}

void ViewModelBase::EndResetModelNotify()
{
  endResetModel();
}

}  // namespace mvvm
