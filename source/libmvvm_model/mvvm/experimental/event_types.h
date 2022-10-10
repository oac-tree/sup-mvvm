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

namespace mvvm::experimental
{

struct DataChangedEvent
{
  int data_role{0};
  SessionItem* item{nullptr};
};

struct AboutToInsertItemEvent
{
  SessionItem* item{nullptr};
  TagIndex tag_index;
};

struct ItemInsertedEvent
{
  SessionItem* item{nullptr};
  TagIndex tag_index;
};

using event_t =
    std::variant<std::monostate, DataChangedEvent, AboutToInsertItemEvent, ItemInsertedEvent>;

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_EVENT_TYPES_H_
