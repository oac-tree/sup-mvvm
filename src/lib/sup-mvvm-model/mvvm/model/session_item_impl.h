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

#include <mvvm/core/variant.h>
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
  SessionItemImpl(std::string item_type, std::unique_ptr<SessionItemData> data,
                  std::unique_ptr<TaggedItems> tags);
  ~SessionItemImpl();

  SessionItemImpl(const SessionItemImpl&) = delete;
  SessionItemImpl& operator=(const SessionItemImpl&) = delete;
  SessionItemImpl(SessionItemImpl&&) = delete;
  SessionItemImpl& operator=(SessionItemImpl&&) = delete;

  std::string GetType() const;

  variant_t Data(std::int32_t role);

  bool SetData(const variant_t& value, std::int32_t role);

  SessionItemData* GetItemData();

  TaggedItems* GetTaggedItems();

  SessionItem* GetParent();

  void SetParent(SessionItem* parent);

  ISessionModel* GetModel();

  /**
   * @brief Sets the model to implementation owner and all its children.
   *
   * Previous model will be removed, all connections unsubscribed.
   *
   * @param model The model to set.
   * @param my_owner The owner of this implementation.
   */
  void SetModel(ISessionModel* model, SessionItem* my_owner);

  /**
   * @brief Returns a slot associated with this item.
   */
  Slot* GetSlot();

private:
  std::string m_item_type;
  std::unique_ptr<SessionItemData> m_item_data;
  std::unique_ptr<TaggedItems> m_tagged_items;
  SessionItem* m_parent{nullptr};
  ISessionModel* m_model{nullptr};

  //!< slot is used when this item wants to receive someone's notifications
  std::unique_ptr<Slot> m_slot;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSION_ITEM_IMPL_H_
