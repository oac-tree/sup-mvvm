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

#include "mvvm/widgets/property_grid_controller.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/delegates/viewmodel_delegate.h>

#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QLabel>
#include <QStyleOptionViewItem>

namespace mvvm
{

PropertyGridController::PropertyGridController(QAbstractItemModel *model, QObject *parent)
    : QObject(parent), m_view_model(model)
{
  if (!m_view_model)
  {
    throw NullArgumentException("Uninitialized model");
  }
  SetupConnections(m_view_model);
}

PropertyGridController::~PropertyGridController() = default;

std::unique_ptr<QWidget> PropertyGridController::CreateWidget(const QModelIndex &index)
{
  return IsLabel(index) ? CreateLabel(index) : CreateEditor(index);
}

std::vector<PropertyGridController::widget_row_t> PropertyGridController::CreateWidgetGrid()
{
  std::vector<PropertyGridController::widget_row_t> result;

  UpdateMappers();

  result.resize(m_view_model->rowCount());
  for (int row = 0; row < m_view_model->rowCount(); ++row)
  {
    for (int col = 0; col < m_view_model->columnCount(); ++col)
    {
      const auto index = m_view_model->index(row, col);

      auto widget = CreateWidget(index);
      auto &mapper = m_widget_mappers.at(static_cast<size_t>(row));

      if (IsLabel(index))
      {
        // Workaround for QTBUG-10672
        // QDataWidgetMapper does not update non-editable fields
        mapper->addMapping(widget.get(), col, "text");
      }
      else
      {
        mapper->addMapping(widget.get(), col);
      }

      result[row].push_back(std::move(widget));
    }
  }

  return result;
}

bool PropertyGridController::Submit()
{
  bool is_success(true);
  for (auto &mapper : m_widget_mappers)
  {
    is_success |= mapper->submit();
  }
  return is_success;
}

void PropertyGridController::ClearContent()
{
  m_widget_mappers.clear();
  m_delegates.clear();
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
  if (index.row() >= m_delegates.size())
  {
    throw RuntimeException("Error in delegates");
  }
  auto delegate = m_delegates.at(index.row()).get();
  QStyleOptionViewItem view_item;
  auto result = std::unique_ptr<QWidget>(delegate->createEditor(nullptr, view_item, index));
  delegate->setEditorData(result.get(), index);
  return result;
}

//! Update internal mappers for new model layout.

void PropertyGridController::OnLayoutChange()
{
  ClearContent();
  emit GridChanged();
}

void PropertyGridController::UpdateMappers()
{
  ClearContent();

  for (int row = 0; row < m_view_model->rowCount(); ++row)
  {
    auto mapper = std::make_unique<QDataWidgetMapper>();
    auto delegate = std::make_unique<ViewModelDelegate>();
    mapper->setModel(m_view_model);
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());
    mapper->setCurrentModelIndex(m_view_model->index(row, 0));
    m_widget_mappers.emplace_back(std::move(mapper));
    m_delegates.emplace_back(std::move(delegate));
  }
}

void PropertyGridController::SetupConnections(QAbstractItemModel *model)
{
  auto on_row_inserted = [this](const QModelIndex &, int, int) { OnLayoutChange(); };
  connect(model, &QAbstractItemModel::rowsInserted, on_row_inserted);

  auto on_row_removed = [this](const QModelIndex &, int, int) { OnLayoutChange(); };
  connect(model, &QAbstractItemModel::rowsRemoved, on_row_removed);

  connect(model, &QAbstractItemModel::modelReset, this, [this]() { OnLayoutChange(); });
}

}  // namespace mvvm
