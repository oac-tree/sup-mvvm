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

#ifndef MVVM_STANDARDITEMS_GROUP_ITEM_H_
#define MVVM_STANDARDITEMS_GROUP_ITEM_H_

#include <mvvm/model/session_item.h>

#include <cstdint>
#include <string>
#include <vector>

namespace mvvm
{

/**
 * @brief The GroupItem class holds a collection of predefined items with only one of them being
 * "current" at any given moment.
 *
 * The class is intended for inheritance. A concrete subclass populates the group in its constructor
 * via AddToGroup. The current selection is stored as a ComboProperty on the item's data role, which
 * allows the user to switch between alternatives from a combo editor.
 */
class GroupItem : public SessionItem
{
public:
  static constexpr std::string_view kGroupTag = "kGroupTag";

  /**
   * @brief Returns index of the currently selected item, or -1 if the group is empty.
   */
  std::int32_t GetCurrentIndex() const;

  /**
   * @brief Returns currently selected item cast to the given type, or nullptr if the group is empty
   * (const version).
   *
   * Will throw, if the current item can't be cast to the requested type.
   */
  template <typename T = SessionItem>
  const T* GetCurrentItem() const;

  /**
   * @brief Returns currently selected item cast to the given type, or nullptr if the group is empty.
   *
   * Will throw, if the current item can't be cast to the requested type.
   */
  template <typename T = SessionItem>
  T* GetCurrentItem();

  /**
   * @brief Returns the type of the currently selected item, or an empty string if the group is
   * empty.
   */
  std::string GetCurrentType() const;

  /**
   * @brief Sets the current selection to the item with the given type.
   *
   * Will throw, if no item of the given type belongs to the group.
   */
  void SetCurrentType(const std::string& item_type);

  /**
   * @brief Sets the current selection to the item of the given type and returns it.
   *
   * Will throw, if no item of the given type belongs to the group.
   *
   * @tparam T The type of the item to select.
   * @return The now-current item cast to the given type.
   */
  template <typename T>
  T* SetCurrentType();

protected:
  explicit GroupItem(const std::string& item_type);

  /**
   * @brief Sets the current selection to the item with the given index.
   */
  void SetCurrentIndex(std::int32_t index);

  /**
   * @brief Adds an item of a given type to the group.
   *
   * @param text The text shown in a combo editor when selecting an item in a group. If empty, the
   * type of the item is used.
   * @param make_selected Defines whether the item should be selected by default.
   */
  template <typename T>
  void AddToGroup(const std::string& text = {}, bool make_selected = false);

  /**
   * @brief Updates internal combo property representing the list of items and current selection.
   */
  void UpdateCombo();

private:
  std::int32_t m_index_to_select{0};
  std::vector<std::string> m_item_text;
};

template <typename T>
const T* GroupItem::GetCurrentItem() const
{
  return GetCurrentIndex() < 0
             ? nullptr
             : GetItem<T>(TagIndex(kGroupTag, static_cast<std::size_t>(GetCurrentIndex())));
}

template <typename T>
T* GroupItem::GetCurrentItem()
{
  return const_cast<T*>(static_cast<const GroupItem*>(this)->GetCurrentItem<T>());
}

template <typename T>
T* GroupItem::SetCurrentType()
{
  SetCurrentType(T().GetType());
  return GetCurrentItem<T>();
}

template <typename T>
void GroupItem::AddToGroup(const std::string& text, bool make_selected)
{
  m_item_text.push_back(text.empty() ? T().GetType() : text);
  (void)InsertItem<T>(TagIndex::Append(std::string(kGroupTag)));
  if (make_selected)
  {
    m_index_to_select = static_cast<std::int32_t>(m_item_text.size()) - 1;
  }
  UpdateCombo();
}

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_GROUP_ITEM_H_
