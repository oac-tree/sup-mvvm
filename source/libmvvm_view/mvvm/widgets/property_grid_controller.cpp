/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/widgets/property_grid_controller.h"

#include <mvvm/delegates/viewmodel_delegate.h>

#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QStyleOptionViewItem>

namespace mvvm
{

PropertyGridController::PropertyGridController(QAbstractItemModel *model, QObject *parent)
    : QObject(parent), m_view_model(model), m_delegate(std::make_unique<ViewModelDelegate>())
{
  SetupConnections(m_view_model);
}

PropertyGridController::~PropertyGridController() = default;

std::unique_ptr<QWidget> PropertyGridController::CreateWidget(const QModelIndex &index)
{
  QStyleOptionViewItem view_item;
  return std::unique_ptr<QWidget>(m_delegate->createEditor(nullptr, view_item, index));
}

std::vector<PropertyGridController::widget_row_t> PropertyGridController::CreateGrid() const
{
  return {};
}

//! Update internal mappers for new model layout.

void PropertyGridController::UpdateGrid()
{
  UpdateMappers();

  emit GridChanged();
}

void PropertyGridController::UpdateMappers()
{
  m_widget_mappers.clear();

  for (int row = 0; row < m_view_model->rowCount(); ++row)
  {
    auto mapper = std::make_unique<QDataWidgetMapper>();
    mapper->setModel(m_view_model);
    mapper->setItemDelegate(m_delegate.get());
    mapper->setRootIndex(QModelIndex());
    mapper->setCurrentModelIndex(m_view_model->index(row, 0));
    m_widget_mappers.emplace_back(std::move(mapper));
  }
}

void PropertyGridController::SetupConnections(QAbstractItemModel *model)
{
  auto on_row_inserted = [this](const QModelIndex &, int, int) { UpdateGrid(); };
  connect(m_view_model, &QAbstractItemModel::rowsInserted, on_row_inserted);

  auto on_row_removed = [this](const QModelIndex &, int, int) { UpdateGrid(); };
  connect(m_view_model, &QAbstractItemModel::rowsRemoved, on_row_removed);
}

}  // namespace mvvm
