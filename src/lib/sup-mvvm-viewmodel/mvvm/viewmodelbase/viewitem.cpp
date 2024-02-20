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

#include <mvvm/core/exceptions.h>
#include <mvvm/utils/container_utils.h>

#include <algorithm>
#include <vector>

namespace mvvm
{

/**
 * @brief The ViewItemImpl class is a pointer implementation (pimpl) for ViewItem.
 */
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

  void AppendRow(std::vector<std::unique_ptr<ViewItem>> items)
  {
    InsertRow(m_rows, std::move(items));
    UpdateChildrenCache();
  }

  void InsertRow(int row, std::vector<std::unique_ptr<ViewItem>> items)
  {
    if (items.empty())
    {
      throw RuntimeException("ViewItem: attempt to insert empty row");
    }

    if (m_columns > 0 && items.size() != static_cast<size_t>(m_columns))
    {
      throw RuntimeException("ViewItem: wrong number of columns");
    }

    if (row < 0 || row > m_rows)
    {
      throw RuntimeException("ViewItem: invalid row index");
    }

    m_children.insert(std::next(m_children.begin(), row * m_columns),
                      std::make_move_iterator(items.begin()), std::make_move_iterator(items.end()));

    m_columns = static_cast<int>(items.size());
    ++m_rows;

    UpdateChildrenCache();
  }

  void RemoveRow(int row)
  {
    if (row < 0 || row >= m_rows)
    {
      throw RuntimeException("Error in ViewItem: invalid row index.");
    }

    auto begin = std::next(m_children.begin(), row * m_columns);
    m_children.erase(begin, std::next(begin, m_columns));
    --m_rows;
    if (m_rows == 0)
    {
      m_columns = 0;
    }

    UpdateChildrenCache();
  }

  ViewItem* GetChild(int row, int column) const
  {
    if (row < 0 || row >= m_rows)
    {
      throw RuntimeException("ViewItem: wrong row");
    }

    if (column < 0 || column >= m_columns)
    {
      throw RuntimeException("ViewItem: wrong column");
    }

    return m_children.at(static_cast<size_t>(column + row * m_columns)).get();
  }

  int GetIndexOfChild(const ViewItem* child)
  {
    return utils::IndexOfItem(m_children.begin(), m_children.end(), child);
  }

  /**
   * @brief Returns vector of children.
   */
  std::vector<ViewItem*> GetChildren() const
  {
    std::vector<ViewItem*> result;
    std::transform(m_children.begin(), m_children.end(), std::back_inserter(result),
                   [](const auto& child_uptr) { return child_uptr.get(); });
    return result;
  }

  /**
   * @brief Updates cached row,col values for children.
   */
  void UpdateChildrenCache()
  {
    for (size_t index = 0; index < m_children.size(); ++index)
    {
      const int row = index / m_columns;
      const int col = index % m_columns;
      m_children[index]->UpdatePositionCache(row, col);
    }
  }
};

ViewItem::ViewItem() : ViewItem(std::make_unique<ViewItemData>()) {}

ViewItem::ViewItem(std::unique_ptr<ViewItemDataInterface> view_item_data)
    : p_impl(std::make_unique<ViewItemImpl>(std::move(view_item_data)))
{
}

ViewItem::~ViewItem() = default;

int ViewItem::GetRowCount() const
{
  return p_impl->m_rows;
}

int ViewItem::GetColumnCount() const
{
  return p_impl->m_columns;
}

void ViewItem::AppendRow(std::vector<std::unique_ptr<ViewItem>> items)
{
  for (auto& x : items)
  {
    x->SetParent(this);
  }
  p_impl->AppendRow(std::move(items));
}

void ViewItem::InsertRow(int row, std::vector<std::unique_ptr<ViewItem>> items)
{
  for (auto& x : items)
  {
    x->SetParent(this);
  }
  p_impl->InsertRow(row, std::move(items));
}

void ViewItem::RemoveRow(int row)
{
  p_impl->RemoveRow(row);
}

void ViewItem::Clear()
{
  p_impl->m_children.clear();
  p_impl->m_rows = 0;
  p_impl->m_columns = 0;
}

ViewItem* ViewItem::GetParent() const
{
  return p_impl->m_parent;
}

ViewItem* ViewItem::GetChild(int row, int column) const
{
  return p_impl->GetChild(row, column);
}

ViewItemDataInterface* ViewItem::GetItemData()
{
  return const_cast<ViewItemDataInterface*>(static_cast<const ViewItem*>(this)->GetItemData());
}

const ViewItemDataInterface* ViewItem::GetItemData() const
{
  return p_impl->m_view_item_data.get();
}

int ViewItem::Row() const
{
  return p_impl->m_my_row;
}

int ViewItem::Column() const
{
  return p_impl->m_my_col;
}

QVariant ViewItem::data(int qt_role) const
{
  return GetItemData() ? GetItemData()->Data(qt_role) : QVariant();
}

bool ViewItem::setData(const QVariant& value, int qt_role)
{
  return GetItemData() ? GetItemData()->SetData(value, qt_role) : false;
}

Qt::ItemFlags ViewItem::flags() const
{
  Qt::ItemFlags result = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  auto item_data = GetItemData();
  if (item_data && item_data->IsEditable() && item_data->Data(Qt::EditRole).isValid())
  {
    result |= Qt::ItemIsEditable;
  }

  return result;
}

std::vector<ViewItem*> ViewItem::GetChildren() const
{
  return p_impl->GetChildren();
}

void ViewItem::SetParent(ViewItem* parent)
{
  p_impl->m_parent = parent;
}

void ViewItem::UpdatePositionCache(int row, int col)
{
  p_impl->m_my_row = row;
  p_impl->m_my_col = col;
}

}  // namespace mvvm
