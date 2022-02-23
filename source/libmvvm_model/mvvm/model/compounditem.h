/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
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

#ifndef MVVM_MODEL_COMPOUNDITEM_H
#define MVVM_MODEL_COMPOUNDITEM_H

#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/taginfo.h"
//#include "mvvm/utils/reallimits.h"

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

  //! Adds and item of given type, and registers it under the given `name`.
  //! Beneath will create a tag intended to store a single item without a possibility to remove.
  template <typename T>
  T* AddBranch(const std::string& name);

  //! Adds an item of a given type and register it under the given 'name' as a property item.
  //! A property item is an item with the following features: it can't be removed, it appears in
  //! property editors, it doesn't appear in a list of top-level items.
  template <typename T>
  T* AddProperty(const std::string& name);

  //! Adds PropertyItem and sets its value to 'value'.
  template <typename V>
  PropertyItem* AddProperty(const std::string& name, const V& value);
  PropertyItem* AddProperty(const std::string& name, const char* value);

  std::string GetDisplayName() const override;

  template <typename T>
  T Property(const std::string& tag) const;

  template <typename T>
  void SetProperty(const std::string& tag, const T& value);
};

template <typename T>
T* CompoundItem::AddBranch(const std::string& name)
{
  RegisterTag(TagInfo::CreatePropertyTag(name, T().GetType()));
  auto result = InsertItem<T>({name, 0});
  return result;
}

template <typename T>
T* CompoundItem::AddProperty(const std::string& name)
{
  auto result = AddBranch<T>(name);
  result->SetDisplayName(name);
  result->SetAppearanceFlag(kProperty, true);
  return result;
}

template <typename V>
PropertyItem* CompoundItem::AddProperty(const std::string& name, const V& value)
{
  auto property = AddProperty<PropertyItem>(name);
  property->SetData(value);
  // FIXME uncomment
  //    if constexpr (std::is_floating_point_v<V>)
  //        property->setData(RealLimits::limitless(), ItemDataRole::LIMITS);
  return property;
}

//! Returns data stored in property item.
//! Property is single item registered under certain tag via CompoundItem::AddProperty method.

template <typename T>
inline T CompoundItem::Property(const std::string& tag) const
{
  return GetItem(tag)->Data<T>();
}

//! Sets value to property item.
//! Property is single item registered under certain tag via CompoundItem::AddProperty method, the
//! value will be assigned to it's data role.

template <typename T>
inline void CompoundItem::SetProperty(const std::string& tag, const T& value)
{
  GetItem(tag)->SetData(value);
}

}  // namespace mvvm

#endif  // MVVM_MODEL_COMPOUNDITEM_H
