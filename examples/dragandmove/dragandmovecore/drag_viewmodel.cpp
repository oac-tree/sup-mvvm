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

#include "drag_viewmodel.h"

#include <mvvm/model/session_model.h>
#include <mvvm/utils/numeric_utils.h>
#include <mvvm/viewmodel/qtcore_helper.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

#include <QByteArray>
#include <QDataStream>
#include <QMimeData>

namespace
{
const QString AppMimeType = "application/org.cppmvvm.moveitem";
}  // namespace

namespace dragandmove
{

DragViewModel::DragViewModel(mvvm::SessionModel* model, QObject* parent)
    : PropertyTableViewModel(model, parent)
{
}

Qt::ItemFlags DragViewModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags defaultFlags = PropertyTableViewModel::flags(index);
  if (index.isValid())
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
  return Qt::ItemIsDropEnabled | defaultFlags;
}

QMimeData* DragViewModel::mimeData(const QModelIndexList& index_list) const
{
  auto mimeData = new QMimeData;
  auto items = mvvm::utils::ParentItemsFromIndex(index_list);

  // Saving list of SessionItem's identifiers related to all DemoItem

  QStringList identifiers;
  for (auto item : mvvm::utils::ParentItemsFromIndex(index_list))
  {
    identifiers.append(QString::fromStdString(item->GetIdentifier()));
  }

  mimeData->setData(AppMimeType, mvvm::utils::GetByteArray(identifiers));
  return mimeData;
}

Qt::DropActions DragViewModel::supportedDragActions() const
{
  return Qt::TargetMoveAction;
}

Qt::DropActions DragViewModel::supportedDropActions() const
{
  return Qt::TargetMoveAction;
}

bool DragViewModel::canDropMimeData(const QMimeData* data, Qt::DropAction, int, int,
                                    const QModelIndex&) const
{
  return data->hasFormat(AppMimeType);
}

bool DragViewModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                                 const QModelIndex& parent)
{
  if (!canDropMimeData(data, action, row, column, parent))
  {
    return false;
  }

  int requested_row = parent.isValid() ? parent.row() : row;

  // retrieving list of item identifiers and accessing items
  auto identifiers = mvvm::utils::GetStringList(data->data(AppMimeType));
  for (const auto& id : identifiers)
  {
    auto item = GetRootSessionItem()->GetModel()->FindItem(id.toStdString());

    int max_item_index = item->GetParent()->GetItemCount(item->GetTagIndex().GetTag()) - 1;

    int row = std::clamp(requested_row, 0, max_item_index);
    GetRootSessionItem()->GetModel()->MoveItem(item, GetRootSessionItem(),
                                               mvvm::TagIndex::Default(row));
  }

  return false;
}

}  // namespace dragandmove
