/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Authors       : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "viewitem.h"

#include "viewitemdata.h"

#include "mvvm/utils/containerutils.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

using namespace ModelView;

struct ViewItem::ViewItemImpl
{
  std::vector<std::unique_ptr<ViewItem>> m_children;  //! buffer to hold rows x columns
  int m_rows{0};
  int m_columns{0};
  std::unique_ptr<ViewItemDataInterface> m_view_item_data;
  ViewItem* m_self{nullptr};

  ViewItemImpl(std::unique_ptr<ViewItemDataInterface> view_item_data)
      : m_view_item_data(std::move(view_item_data))
  {
  }

  void appendRow(std::vector<std::unique_ptr<ViewItem>> items)
  {
    insertRow(m_rows, std::move(items));
  }

  void insertRow(int row, std::vector<std::unique_ptr<ViewItem>> items)
  {
    if (items.empty())
      throw std::runtime_error("Error in ViewItemImpl: attempt to insert empty row");

    if (m_columns > 0 && items.size() != static_cast<size_t>(m_columns))
      throw std::runtime_error("Error in ViewItemImpl: wrong number of columns.");

    if (row < 0 || row > m_rows)
    {
      throw std::runtime_error("Error in ViewItemImpl: invalid row index.");
    }

    m_children.insert(std::next(m_children.begin(), row * m_columns),
                      std::make_move_iterator(items.begin()), std::make_move_iterator(items.end()));

    m_columns = static_cast<int>(items.size());
    ++m_rows;
  }

  void removeRow(int row)
  {
    if (row < 0 || row >= m_rows)
      throw std::runtime_error("Error in ViewItem: invalid row index.");

    auto begin = std::next(m_children.begin(), row * m_columns);
    auto end = std::next(begin, m_columns);
    m_children.erase(begin, end);
    --m_rows;
    if (m_rows == 0)
      m_columns = 0;
  }

  ViewItem* child(int row, int column) const
  {
    if (row < 0 || row >= m_rows)
      throw std::runtime_error("Error in ViewItem: wrong row)");

    if (column < 0 || column >= m_columns)
      throw std::runtime_error("Error in ViewItem: wrong column)");

    return m_children.at(static_cast<size_t>(column + row * m_columns)).get();
  }

  ViewItem* parent() { return m_self; }

  int index_of_child(const ViewItem* child)
  {
    return Utils::IndexOfItem(m_children.begin(), m_children.end(), child);
  }

  //! Returns vector of children.

  std::vector<ViewItem*> get_children() const
  {
    std::vector<ViewItem*> result;
    std::transform(m_children.begin(), m_children.end(), std::back_inserter(result),
                   [](const auto& x) { return x.get(); });
    return result;
  }
};

ViewItem::ViewItem() : ViewItem(std::make_unique<ViewItemData>()) {}

ViewItem::ViewItem(std::unique_ptr<ViewItemDataInterface> view_item_data)
    : p_impl(std::make_unique<ViewItemImpl>(std::move(view_item_data)))
{
}

ViewItem::~ViewItem() = default;

//! Returns the number of child item rows that the item has.

int ViewItem::rowCount() const
{
  return p_impl->m_rows;
}

//! Returns the number of child item columns that the item has.

int ViewItem::columnCount() const
{
  return p_impl->m_columns;
}

//! Appends a row containing items. Number of items should be the same as
//! columnCount() (if there are already some rows). If it is a first row, then
//! items can be of any size.

void ViewItem::appendRow(std::vector<std::unique_ptr<ViewItem>> items)
{
  for (auto& x : items) x->setParent(this);
  p_impl->appendRow(std::move(items));
}

//! Insert a row of items at index 'row'.

void ViewItem::insertRow(int row, std::vector<std::unique_ptr<ViewItem>> items)
{
  for (auto& x : items) x->setParent(this);
  p_impl->insertRow(row, std::move(items));
}

//! Removes row of items at given 'row'. Items will be deleted.

void ViewItem::removeRow(int row)
{
  p_impl->removeRow(row);
}

void ViewItem::clear()
{
  p_impl->m_children.clear();
  p_impl->m_rows = 0;
  p_impl->m_columns = 0;
}

ViewItem* ViewItem::parent() const
{
  return p_impl->parent();
}

ViewItem* ViewItem::child(int row, int column) const
{
  return p_impl->child(row, column);
}

ViewItemDataInterface* ViewItem::item()
{
  return const_cast<ViewItemDataInterface*>(static_cast<const ViewItem*>(this)->item());
}

const ViewItemDataInterface* ViewItem::item() const
{
  return p_impl->m_view_item_data.get();
}

//! Returns the row where the item is located in its parent's child table, or -1
//! if the item has no parent.

int ViewItem::row() const
{
  auto index = parent() ? parent()->p_impl->index_of_child(this) : -1;
  return index >= 0 ? index / parent()->p_impl->m_columns : -1;
}

//! Returns the column where the item is located in its parent's child table, or
//! -1 if the item has no parent.

int ViewItem::column() const
{
  auto index = parent() ? parent()->p_impl->index_of_child(this) : -1;
  return index >= 0 ? index % parent()->p_impl->m_columns : -1;
}

//! Returns the data for given role according to Qt::ItemDataRole namespace
//! definitions.

QVariant ViewItem::data(int qt_role) const
{
  return item() ? item()->data(qt_role) : QVariant();
}

//! Sets the data to underlying SessionItem.

bool ViewItem::setData(const QVariant& value, int qt_role)
{
  return item() ? item()->setData(value, qt_role) : false;
}

//! Returns Qt's item flags.
//! Converts internal SessionItem's status enable/disabled/readonly to what Qt
//! expects.

Qt::ItemFlags ViewItem::flags() const
{
  Qt::ItemFlags result = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  if (item() && item()->isEditable() && item()->isEnabled() && item()->data().isValid())
    result |= Qt::ItemIsEditable;

  return result;
}

std::vector<ViewItem*> ViewItem::children() const
{
  return p_impl->get_children();
}

void ViewItem::setParent(ViewItem* parent)
{
  p_impl->m_self = parent;
}
