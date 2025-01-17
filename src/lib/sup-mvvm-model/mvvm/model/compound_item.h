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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_COMPOUND_ITEM_H_
#define MVVM_MODEL_COMPOUND_ITEM_H_

#include <mvvm/model/property_item.h>
#include <mvvm/model/taginfo.h>

namespace mvvm
{
//! A compound item with convenient methods for adding properties.
//! The property is a single PropertyItem or another CompoundItem inserted into the named container
//! and carrying the data. There can be only one property item associated with the property name,
//! and it can not be removed from the parent.

class MVVM_MODEL_EXPORT CompoundItem : public SessionItem
{
public:
  static inline const std::string Type = "Compound";

  explicit CompoundItem(const std::string& item_type = Type);

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Adds and item of given type, and registers it under the given name as branch item.
   *
   * @param name The name of the tag to register.
   * @return Reference to just created item.
   *
   * @details A branch item is an item which is normally shown in top-level item hierarchies, but
   * hidden from property editors. Once inserted, it can't be removed.
   */
  template <typename T>
  T& AddBranch(const std::string& name);

  /**
   * @brief Adds an item of a given type and register it under the given 'name' as a property item.
   *
   * @param name The name of the tag to register.
   * @return Reference to just created item.
   *
   * @details A property item is an item which is normally hidden in top-level item hierarchies, but
   * shown in property editors. Once inserted, it can't be removed.
   */
  template <typename T>
  T& AddProperty(const std::string& name);

  /**
   * @brief Adds property item and sets its value.
   */
  template <typename V>
  PropertyItem& AddProperty(const std::string& name, const V& value);

  /**
   * @brief Adds property item and sets its value (specialised for char).
   *
   * @details Specialized version for const char is introduced to avoid "const char" conversion into
   variant<bool>.
   */
  PropertyItem& AddProperty(const std::string& name, const char* value);

  /**
   * @brief Returns item display name.
   */
  std::string GetDisplayName() const override;

  /**
   * @brief Returns value stored in property item.
   *
   * @param tag The name under which corresponding property item is registered.
   * @tparam T The type to cast the value.
   *
   * @return The property value.
   */
  template <typename T>
  T Property(const std::string& tag) const;

  /**
   * @brief Sets the value of the property.
   *
   * @param tag The name under which corresponding property item is registered.
   * @param value The value to set.
   */
  template <typename T>
  void SetProperty(const std::string& tag, const T& value);
};

template <typename T>
T& CompoundItem::AddBranch(const std::string& name)
{
  RegisterTag(TagInfo::CreatePropertyTag(name, T().GetType()));
  auto result = InsertItem<T>({name, 0});
  return *result;
}

template <typename T>
T& CompoundItem::AddProperty(const std::string& name)
{
  // A property item is a branch with additional kProperty flag. Properties are normally shown in
  // property editors and hidden in top-level-items editors.
  auto& result = AddBranch<T>(name);
  result.SetDisplayName(name);
  result.SetFlag(Appearance::kProperty, true);
  return result;
}

template <typename V>
PropertyItem& CompoundItem::AddProperty(const std::string& name, const V& value)
{
  auto& property = AddProperty<PropertyItem>(name);
  property.SetData(value);
  return property;
}

template <typename T>
inline T CompoundItem::Property(const std::string& tag) const
{
  return GetItem(tag)->Data<T>();
}

template <typename T>
inline void CompoundItem::SetProperty(const std::string& tag, const T& value)
{
  GetItem(tag)->SetData(value);
}

}  // namespace mvvm

#endif  // MVVM_MODEL_COMPOUND_ITEM_H_
