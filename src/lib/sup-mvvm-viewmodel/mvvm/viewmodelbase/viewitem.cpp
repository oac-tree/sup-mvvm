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

#include "viewitem.h"

#include "viewitem_data.h"

#include <mvvm/utils/container_utils.h>

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace mvvm
{
struct ViewItem::ViewItemImpl
{
  std::vector<std::unique_ptr<ViewItem>> m_children;  //! buffer to hold rows x columns
  int m_rows{0};
  int m_columns{0};
  int m_my_row{-1};
  int m_my_col{-1};
  std::unique_ptr<ViewItemDataInterface> m_view_item_data;
  ViewItem* m_parent{nullptr};

  explicit ViewItemImpl(std::unique_ptr<ViewItemDataInterface> view_item_data)
      : m_view_item_data(std::move(view_item_data))
  {
  }

  void appendRow(std::vector<std::unique_ptr<ViewItem>> items)
  {
    insertRow(m_rows, std::move(items));
    updateChildrenCache();
  }

  void insertRow(int row, std::vector<std::unique_ptr<ViewItem>> items)
  {
    if (items.empty())
    {
      throw std::runtime_error("Error in ViewItemImpl: attempt to insert empty row");
    }

    if (m_columns > 0 && items.size() != static_cast<size_t>(m_columns))
    {
      throw std::runtime_error("Error in ViewItemImpl: wrong number of columns.");
    }

    if (row < 0 || row > m_rows)
    {
      throw std::runtime_error("Error in ViewItemImpl: invalid row index.");
    }

    m_children.insert(std::next(m_children.begin(), row * m_columns),
                      std::make_move_iterator(items.begin()), std::make_move_iterator(items.end()));

    m_columns = static_cast<int>(items.size());
    ++m_rows;

    updateChildrenCache();
  }

  void removeRow(int row)
  {
    if (row < 0 || row >= m_rows)
    {
      throw std::runtime_error("Error in ViewItem: invalid row index.");
    }

    auto begin = std::next(m_children.begin(), row * m_columns);
    m_children.erase(begin, std::next(begin, m_columns));
    --m_rows;
    if (m_rows == 0)
    {
      m_columns = 0;
    }

    updateChildrenCache();
  }

  ViewItem* child(int row, int column) const
  {
    if (row < 0 || row >= m_rows)
    {
      throw std::runtime_error("Error in ViewItem: wrong row)");
    }

    if (column < 0 || column >= m_columns)
    {
      throw std::runtime_error("Error in ViewItem: wrong column)");
    }

    return m_children.at(static_cast<size_t>(column + row * m_columns)).get();
  }

  int index_of_child(const ViewItem* child)
  {
    return utils::IndexOfItem(m_children.begin(), m_children.end(), child);
  }

  //! Returns vector of children.

  std::vector<ViewItem*> get_children() const
  {
    std::vector<ViewItem*> result;
    std::transform(m_children.begin(), m_children.end(), std::back_inserter(result),
                   [](const auto& x) { return x.get(); });
    return result;
  }

  /**
   * @brief Updates cached row,col values for children.
   */
  void updateChildrenCache()
  {
    for (size_t index = 0; index < m_children.size(); ++index)
    {
      const int row = index / m_columns;
      const int col = index % m_columns;
      m_children[index]->updatePositionCache(row, col);
    }
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
  for (auto& x : items)
  {
    x->setParent(this);
  }
  p_impl->appendRow(std::move(items));
}

//! Insert a row of items at index 'row'.

void ViewItem::insertRow(int row, std::vector<std::unique_ptr<ViewItem>> items)
{
  for (auto& x : items)
  {
    x->setParent(this);
  }
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
  return p_impl->m_parent;
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
  return p_impl->m_my_row;
}

//! Returns the column where the item is located in its parent's child table, or
//! -1 if the item has no parent.

int ViewItem::column() const
{
  return p_impl->m_my_col;
}

//! Returns the data for given role according to Qt::ItemDataRole namespace
//! definitions.

QVariant ViewItem::data(int qt_role) const
{
  return item() ? item()->Data(qt_role) : QVariant();
}

//! Sets the data to underlying PresentationItem.

bool ViewItem::setData(const QVariant& value, int qt_role)
{
  return item() ? item()->SetData(value, qt_role) : false;
}

//! Returns Qt's item flags.
//! Converts internal SessionItem's status enable/disabled/readonly to what Qt
//! expects.

Qt::ItemFlags ViewItem::flags() const
{
  Qt::ItemFlags result = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  if (item() && item()->IsEditable() && item()->IsEnabled() && item()->Data(Qt::EditRole).isValid())
  {
    result |= Qt::ItemIsEditable;
  }

  return result;
}

std::vector<ViewItem*> ViewItem::children() const
{
  return p_impl->get_children();
}

void ViewItem::setParent(ViewItem* parent)
{
  p_impl->m_parent = parent;
}

void ViewItem::updatePositionCache(int row, int col)
{
  p_impl->m_my_row = row;
  p_impl->m_my_col = col;
}

}  // namespace mvvm
