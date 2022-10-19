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

#ifndef MVVM_EXPERIMENTAL_EVENT_TYPES_H_
#define MVVM_EXPERIMENTAL_EVENT_TYPES_H_

//! @file event_types.h
//! Defines collection of event types.

#include <mvvm/model/tagindex.h>

#include <variant>

namespace mvvm
{
class SessionItem;
}

namespace mvvm
{

//! An event when item's data is changed.

struct DataChangedEvent
{
  int m_data_role{0};
  SessionItem* m_item{nullptr};

  DataChangedEvent(int data_role, SessionItem* item);
  bool operator==(const DataChangedEvent& other) const;
  bool operator!=(const DataChangedEvent& other) const;
};

//! An event when item is about to be inserted in the model.

struct AboutToInsertItemEvent
{
  SessionItem* m_parent{nullptr};
  TagIndex m_tag_index;

  AboutToInsertItemEvent(SessionItem* parent, const TagIndex& tag_index);
  bool operator==(const AboutToInsertItemEvent& other) const;
  bool operator!=(const AboutToInsertItemEvent& other) const;
};

//! An event when item was inserted in the model.

struct ItemInsertedEvent
{
  SessionItem* m_parent{nullptr};
  TagIndex m_tag_index;

  ItemInsertedEvent(SessionItem* item, const TagIndex& tag_index);
  bool operator==(const ItemInsertedEvent& other) const;
  bool operator!=(const ItemInsertedEvent& other) const;
};

//! Variant for all application events.
using event_t = std::variant<DataChangedEvent, AboutToInsertItemEvent, ItemInsertedEvent>;

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_EVENT_TYPES_H_
