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

#ifndef MVVM_MODEL_PROPERTY_ITEM_H_
#define MVVM_MODEL_PROPERTY_ITEM_H_

#include <mvvm/model/sessionitem.h>

namespace mvvm
{

template <typename T>
class Limits;

//! Item to carry concrete editable entity (e.g. 'double' value with limits).
//! Intended for use as a child or CompountItem, not expected to have own children.

class MVVM_MODEL_EXPORT PropertyItem : public SessionItem
{
public:
  static inline const std::string Type = "PropertyItem";
  using SessionItem::SessionItem;

  PropertyItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id = true) const;

  PropertyItem* SetDisplayName(const std::string& name) override;

  template <typename T>
  PropertyItem* SetLimits(const Limits<T>& value);
};

template <typename T>
PropertyItem* PropertyItem::SetLimits(const Limits<T>& value)
{
  SetData(value, DataRole::kLimits);
  return this;
}

}  // namespace mvvm

#endif  // MVVM_MODEL_PROPERTY_ITEM_H_
