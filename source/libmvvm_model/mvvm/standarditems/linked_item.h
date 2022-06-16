/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_STANDARDITEMS_LINKEDITEM_H
#define MVVM_STANDARDITEMS_LINKEDITEM_H

#include <mvvm/model/sessionitem.h>

namespace mvvm
{

//! Item to store a persistent link to other arbitrary items.

//! The identifier of the item intended for linking is stored as DataRole on board of LinkedItem
//! and can be used to find the corresponding item via SessionModel::FindItem machinery.
//! The machinery is persistent and outlives serialization. Can be used to find items in
//! different models. For that being the case, models should use the same ItemPool.

class MVVM_MODEL_EXPORT LinkedItem : public SessionItem
{
public:
  static inline const std::string Type = "Linked";

  LinkedItem();

  void SetLink(const SessionItem* item);

  template <typename T = SessionItem>
  T* Get() const;

private:
  SessionItem* GetLink() const;
};

//! Returns item linked to given item. Works only in model context.

template <typename T>
T* LinkedItem::Get() const
{
  return dynamic_cast<T*>(GetLink());
}

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_LINKEDITEM_H
