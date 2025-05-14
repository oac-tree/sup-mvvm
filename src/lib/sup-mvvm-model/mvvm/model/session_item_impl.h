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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_SESSION_ITEM_IMPL_H_
#define MVVM_MODEL_SESSION_ITEM_IMPL_H_

#include <mvvm/signals/signal_slot_fwd.h>

#include <memory>
#include <string>

namespace mvvm
{

class SessionItem;
class ISessionModel;
class SessionItemData;
class TaggedItems;

/**
 * @brief The SessionItemImpl contains the core components of SessionItem and hides details from
 * others.
 *
 * It holds container with data and containers with items. Provide methods to add/remove children
 * and set the data without notifying the model.
 */
class SessionItemImpl
{
public:
  SessionItemImpl(const std::string& item_type, std::unique_ptr<SessionItemData> data,
                  std::unique_ptr<TaggedItems> tags);
  ~SessionItemImpl();

  SessionItemImpl(const SessionItemImpl&) = delete;
  SessionItemImpl& operator=(const SessionItemImpl&) = delete;
  SessionItemImpl(SessionItemImpl&&) = delete;
  SessionItemImpl& operator=(SessionItemImpl&&) = delete;

  std::string GetType() const;

  SessionItemData* GetItemData();

  TaggedItems* GetTaggedItems();

private:
  std::string m_item_type;
  std::unique_ptr<SessionItemData> m_item_data;
  std::unique_ptr<TaggedItems> m_tagged_items;
  SessionItem* m_parent{nullptr};
  ISessionModel* m_model{nullptr};
  std::unique_ptr<Slot> m_slot;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSION_ITEM_IMPL_H_
