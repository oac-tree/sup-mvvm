/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "viewmodel_base.h"

#include "viewitem.h"

#include <mvvm/core/exceptions.h>

namespace mvvm
{
struct ViewModelBase::ViewModelBaseImpl
{
  ViewModelBase* model{nullptr};
  std::unique_ptr<ViewItem> root;
  explicit ViewModelBaseImpl(ViewModelBase* model) : model(model) {}

  bool IsItemBelongsToModel(ViewItem* item)
  {
    return model->indexFromItem(item).isValid() || item == model->rootItem();
  }
};

ViewModelBase::ViewModelBase(QObject* parent)
    : QAbstractItemModel(parent), p_impl(std::make_unique<ViewModelBaseImpl>(this))
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
  return is_valid_row && is_valid_column ? createIndex(row, column, parent_item->child(row, column))
                                         : QModelIndex();
}

QModelIndex ViewModelBase::parent(const QModelIndex& child) const
{
  if (auto child_item = itemFromIndex(child); child_item)
  {
    auto parent_item = child_item->parent();
    return parent_item == rootItem()
               ? QModelIndex()
               : createIndex(parent_item->row(), parent_item->column(), parent_item);
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
  return item ? item->data(role) : QVariant();
}

bool ViewModelBase::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (!index.isValid())
  {
    return false;
  }

  if (auto item = itemFromIndex(index); item)
  {
    bool result = item->setData(value, role);
    if (result)
    {
      emit dataChanged(index, index, QVector<int>() << role);
    }
    return result;
  }

  return false;
}

//! Returns a pointer to invisible root item.

const ViewItem* ViewModelBase::rootItem() const
{
  return p_impl->root.get();
}

ViewItem* ViewModelBase::rootItem()
{
  return const_cast<ViewItem*>(static_cast<const ViewModelBase*>(this)->rootItem());
}

//! Returns a pointer to the ViewItem associated with the given index.
//! If index is invalid, returns nullptr.

ViewItem* ViewModelBase::itemFromIndex(const QModelIndex& index)
{
  return const_cast<ViewItem*>(static_cast<const ViewModelBase*>(this)->itemFromIndex(index));
}

const ViewItem* ViewModelBase::itemFromIndex(const QModelIndex& index) const
{
  return index.isValid() ? static_cast<const ViewItem*>(index.internalPointer()) : nullptr;
}

//! Returns the QModelIndex associated with the given item.

QModelIndex ViewModelBase::indexFromItem(const ViewItem* item) const
{
  return item && item->parent()
             ? createIndex(item->row(), item->column(), const_cast<ViewItem*>(item))
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
  parent->clear();
  endRemoveRows();
}

//! Insert a row of items at index 'row' to given parent.

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

//! Appends row of items to given parent.

void ViewModelBase::appendRow(ViewItem* parent, std::vector<std::unique_ptr<ViewItem>> items)
{
  insertRow(parent, parent->GetRowCount(), std::move(items));
}

//! Returns the item flags for the given index.

Qt::ItemFlags ViewModelBase::flags(const QModelIndex& index) const
{
  Qt::ItemFlags result = QAbstractItemModel::flags(index);
  if (auto item = itemFromIndex(index); item)
  {
    result |= item->flags();
  }
  return result;
}

//! Sets new root item. Previous item will be deleted, model will be reset.

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

//! Allows to call internal method from outside.

void ViewModelBase::BeginResetModelNotify()
{
  beginResetModel();
}

//! Allows to call internal method from outside.

void ViewModelBase::EndResetModelNotify()
{
  endResetModel();
}

}  // namespace mvvm
