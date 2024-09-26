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

#include "property_grid_controller.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/providers/viewmodel_delegate.h>
#include <mvvm/viewmodel/viewmodel_utils.h>

#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QDebug>
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

std::vector<PropertyGridController::widget_row_t> PropertyGridController::CreateWidgetGrid()
{
  std::vector<PropertyGridController::widget_row_t> result;

  UpdateMappers();

  result.resize(m_view_model->rowCount());
  m_widgets.resize(m_view_model->rowCount());

  for (int row = 0; row < m_view_model->rowCount(); ++row)
  {
    auto &mapper = m_widget_mappers.at(static_cast<size_t>(row));

    for (int col = 0; col < m_view_model->columnCount(); ++col)
    {
      const auto index = m_view_model->index(row, col);

      auto widget = CreateWidget(index);

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
      m_widgets[row].push_back(widget.get());
      result[row].push_back(std::move(widget));
    }

    mapper->setCurrentIndex(row);
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

std::unique_ptr<QWidget> PropertyGridController::CreateWidget(const QModelIndex &index)
{
  return IsLabel(index) ? CreateLabel(index) : CreateEditor(index);
}

void PropertyGridController::ClearContent()
{
  m_widget_mappers.clear();
  m_delegates.clear();
  m_widgets.clear();
}

bool PropertyGridController::IsLabel(const QModelIndex &index) const
{
  return !(m_view_model->flags(index) & Qt::ItemIsEditable);
}

std::unique_ptr<QWidget> PropertyGridController::CreateLabel(const QModelIndex &index)
{
  auto result = std::make_unique<QLabel>(m_view_model->data(index, Qt::DisplayRole).toString());
  result->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed));
  return result;
}

std::unique_ptr<QWidget> PropertyGridController::CreateEditor(const QModelIndex &index)
{
  if (index.row() >= m_delegates.size())
  {
    throw RuntimeException("Error in delegates");
  }

  auto delegate = m_delegates.at(index.row()).get();
  const QStyleOptionViewItem view_item;
  auto result = std::unique_ptr<QWidget>(delegate->createEditor(nullptr, view_item, index));
  return result;
}

void PropertyGridController::OnLayoutChange()
{
  // all we can do currently, is to clear everything and notify the user
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
    mapper->setOrientation(Qt::Horizontal);
    mapper->setModel(m_view_model);
    mapper->setItemDelegate(delegate.get());
    mapper->setRootIndex(QModelIndex());
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

  auto on_data_change =
      [this](const QModelIndex &index, const QModelIndex &, const QVector<int> &roles)
  {
    qDebug() << index << roles;
    QVector<int> expected_roles = {Qt::ForegroundRole};
    if (roles == expected_roles)
    {
      if (auto item = utils::ItemFromIndex(index); item)
      {
        qDebug() << index << item->IsEnabled() << QString::fromStdString(item->GetDisplayName());
        m_widgets[index.row()][index.column()]->setEnabled(item->IsEnabled());
      }
    }
  };
  connect(m_view_model, &QAbstractItemModel::dataChanged, on_data_change);
}

}  // namespace mvvm
