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

#ifndef DRAGANDMOVE_DRAGANDMOVECORE_DRAG_VIEWMODEL_H_
#define DRAGANDMOVE_DRAGANDMOVECORE_DRAG_VIEWMODEL_H_

#include <mvvm/viewmodel/property_table_viewmodel.h>

namespace mvvm
{
class SessionModel;
}

namespace dragandmove
{

//! View model with drag-and-drop support. Relies on PropertyTableViewModel to show
//! properties of DemoItem in table-like views.

class DragViewModel : public mvvm::PropertyTableViewModel
{
  Q_OBJECT

public:
  DragViewModel(mvvm::SessionModel* model, QObject* parent = nullptr);

  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QMimeData* mimeData(const QModelIndexList& index_list) const override;
  Qt::DropActions supportedDragActions() const override;
  Qt::DropActions supportedDropActions() const override;
  bool canDropMimeData(const QMimeData* data, Qt::DropAction, int, int,
                       const QModelIndex&) const override;
  bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                    const QModelIndex& parent) override;
};

}  // namespace dragandmove

#endif  // DRAGANDMOVE_DRAGANDMOVECORE_DRAG_VIEWMODEL_H_
