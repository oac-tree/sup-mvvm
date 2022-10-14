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
#include <QLabel>
#include <QDebug>
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
  return IsLabel(index) ? CreateLabel(index) : CreateEditor(index);
}

std::vector<PropertyGridController::widget_row_t> PropertyGridController::CreateGrid()
{
  std::vector<PropertyGridController::widget_row_t> result;

  UpdateMappers();

  result.resize(m_view_model->rowCount());
  for (int row = 0; row < m_view_model->rowCount(); ++row)
  {
    for (int col = 0; col < m_view_model->columnCount(); ++col)
    {
      auto widget = CreateWidget(m_view_model->index(row, col));
      auto &mapper = m_widget_mappers.at(static_cast<size_t>(row));
      mapper->addMapping(widget.get(), col);
      result[row].push_back(std::move(widget));
    }
  }

  return result;
}

//! Returns true if given cell has to be represented by the label.

bool PropertyGridController::IsLabel(const QModelIndex &index) const
{
  return !(m_view_model->flags(index) & Qt::ItemIsEditable);
}

//! Create a label for a given cell index.

std::unique_ptr<QWidget> PropertyGridController::CreateLabel(const QModelIndex &index)
{
  auto result = std::make_unique<QLabel>(m_view_model->data(index, Qt::DisplayRole).toString());
  result->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
  return result;
}

//! Create an editor for a given cell index.

std::unique_ptr<QWidget> PropertyGridController::CreateEditor(const QModelIndex &index)
{
  QStyleOptionViewItem view_item;
  return std::unique_ptr<QWidget>(m_delegate->createEditor(nullptr, view_item, index));
}

//! Update internal mappers for new model layout.

void PropertyGridController::OnLayoutChange()
{
  qDebug() << "xxxxx";
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
  auto on_row_inserted = [this](const QModelIndex &, int, int) { OnLayoutChange(); };
  connect(model, &QAbstractItemModel::rowsInserted, on_row_inserted);

  auto on_row_removed = [this](const QModelIndex &, int, int) { OnLayoutChange(); };
  connect(model, &QAbstractItemModel::rowsRemoved, on_row_removed);
}

}  // namespace mvvm
