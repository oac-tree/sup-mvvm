/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_MODEL_ITEM_SELECTION_H_
#define MVVM_MODEL_ITEM_SELECTION_H_

#include <mvvm/utils/container_utils.h>

#include <vector>

namespace mvvm
{

class SessionItem;

/**
 * @brief The ItemSelection class stores information about currently selected items.
 *
 * Used to notify widgets in the context of ItemViewComponentProvider.
 */
class ItemSelection
{
public:
  ItemSelection() = default;

  /**
   * @brief Constructor to create selection containing single item.
   *
   * We allow nullptr for convenience, since the widget can report nullptr as a sign of "no
   * selection".
   */
  explicit ItemSelection(const SessionItem* selected_item);

  /**
   * @brief Constructor to create selection containing single item.
   *
   * This "nullptr" version is to avoid collision with templated version.
   *
   * We allow nullptr for convenience, since the widget can report nullptr as a sign of "no
   * selection".
   */
  explicit ItemSelection(std::nullptr_t);

  /**
   * @brief Constructor to create selection from vector of items.
   */
  explicit ItemSelection(const std::vector<const SessionItem*>& selected_items);

  /**
   * @brief Constructor to create selection from any vector of any suitable items.
   */
  template <typename ElementT>
  explicit ItemSelection(const std::vector<ElementT>& selected_items);

  /**
   * @brief Returns currently selected item;
   *
   * If more than one item is selected, will return first one.
   */
  const SessionItem* GetSelectedItem() const;

  /**
   * @brief Returns vector of currently selected items.
   */
  std::vector<const SessionItem*> GetSelectedItems() const;

  /**
   * @brief Returns an item currently selected in a view and casted to specified type.
   */
  template <typename T>
  const T* GetSelected() const;

  /**
   * @brief Returns items currently selected in a view and casted to specified type.
   */
  template <typename T>
  std::vector<const T*> GetSelectedItems() const;

  /**
   * @brief Checks if this selection is the same as another selection.
   *
   * The selections are the same if they contain the same items, regardless of their order.
   */
  bool IsSameSelection(const ItemSelection& other) const;

private:
  std::vector<const SessionItem*> m_items;
};

template <typename ElementT>
inline ItemSelection::ItemSelection(const std::vector<ElementT>& selected_items)
{
  m_items.reserve(selected_items.size());
  for (const auto* item : selected_items)
  {
    m_items.push_back(item);
  }
}

template <typename T>
const T* ItemSelection::GetSelected() const
{
  return dynamic_cast<const T*>(GetSelectedItem());
}

template <typename T>
std::vector<const T*> ItemSelection::GetSelectedItems() const
{
  return utils::CastItems<const T>(GetSelectedItems());
}

bool operator==(const ItemSelection& lhs, const ItemSelection& rhs);
bool operator!=(const ItemSelection& lhs, const ItemSelection& rhs);

}  // namespace mvvm

#endif  // MVVM_MODEL_ITEM_SELECTION_H_
