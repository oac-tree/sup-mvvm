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

#include "session_item.h"

#include "item_utils.h"
#include "make_clone.h"
#include "process_command.h"
#include "session_item_container.h"
#include "session_item_data.h"
#include "session_item_impl.h"
#include "tagged_items.h"
#include "validate_utils.h"

#include <mvvm/commands/insert_item_command.h>
#include <mvvm/commands/remove_item_command.h>
#include <mvvm/commands/set_value_command.h>
#include <mvvm/core/flags.h>
#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/signals/model_event_handler.h>

#include <utility>

namespace mvvm
{

namespace
{

/**
 * @brief Constructs convenience Flags<> object from the underlying value used to store Appearance
 * flags.
 */
Flags<Appearance> GetFlags(const SessionItem& item)
{
  return item.HasData(DataRole::kAppearance)
             ? Flags<Appearance>(item.Data<Flags<Appearance>::StorageT>(DataRole::kAppearance))
             : Flags<Appearance>{};
}

/**
 * @brief Stores given flags on board of given item.
 */
void SetFlags(const Flags<Appearance>& flags, SessionItem& item)
{
  (void)item.SetData(flags.to_ulong(), DataRole::kAppearance);
}

}  // namespace

SessionItem::SessionItem() : SessionItem(GetTypeName<SessionItem>()) {}

SessionItem::SessionItem(const std::string& item_type)
    : SessionItem(item_type, std::make_unique<SessionItemData>(), std::make_unique<TaggedItems>())
{
  utils::SetIdentifier(*this);
}

SessionItem::SessionItem(const std::string& item_type, std::unique_ptr<SessionItemData> data,
                         std::unique_ptr<TaggedItems> tags)
    : p_impl(std::make_unique<SessionItemImpl>(item_type, std::move(data), std::move(tags)))
{
}

void SessionItem::ActivateStrategies()
{
  // nothing to do by default
}

SessionItem::~SessionItem()
{
  if (auto* model = GetModel(); model != nullptr)
  {
    // Notify subscribers that the item is being destroyed. The event handler is absent when the
    // item belongs to a model without notification capabilities, and also during the model's own
    // destruction (the handler is gone by the time items are destroyed); in both cases we simply
    // skip the notification.
    if (auto* event_handler = model->GetEventHandler(); event_handler != nullptr)
    {
      event_handler->Notify<ItemDestroyedEvent>(this);
    }
    model->CheckOut(this);
  }
}

std::unique_ptr<SessionItem> SessionItem::Clone() const
{
  return MakeClone(*this);
}

std::string SessionItem::GetType() const
{
  return p_impl->GetType();
}

std::string SessionItem::GetIdentifier() const
{
  return Data<std::string>(DataRole::kIdentifier);
}

std::string SessionItem::GetDisplayName() const
{
  return HasData(DataRole::kDisplay) ? Data<std::string>(DataRole::kDisplay) : GetType();
}

SessionItem& SessionItem::SetDisplayName(const std::string& name) &
{
  (void)SetData(name, DataRole::kDisplay);
  return *this;
}

ISessionModel* SessionItem::GetModel() const
{
  return p_impl->GetModel();
}

SessionItem* SessionItem::GetParent() const
{
  return p_impl->GetParent();
}

TagIndex SessionItem::GetTagIndex() const
{
  return (GetParent() != nullptr) ? GetParent()->TagIndexOfItem(this) : TagIndex();
}

bool SessionItem::HasData(role_t role) const
{
  return GetItemData()->HasData(role);
}

variant_t SessionItem::GetDataImpl(role_t role) const
{
  return GetItemData()->Data(role);
}

bool SessionItem::SetData(const char* value, role_t role)
{
  return SetData(std::string(value), role);
}

bool SessionItem::SetDataInternal(const variant_t& value, role_t role)
{
  if (m_data_strategy)
  {
    return m_data_strategy(this, value, role);
  }

  if (GetModel() != nullptr)
  {
    return ProcessCommand<SetValueCommand>(*GetModel(), this, value, role);
  }

  return utils::SetDataImpl(*this, value, role);
}

std::size_t SessionItem::GetTotalItemCount() const
{
  return GetTaggedItems()->GetTotalItemCount();
}

std::vector<const SessionItem*> SessionItem::GetAllItems() const
{
  return GetTaggedItems()->GetAllItems();
}

std::vector<SessionItem*> SessionItem::GetAllItems()
{
  return GetTaggedItems()->GetAllItems();
}

std::size_t SessionItem::GetItemCount(std::string_view tag) const
{
  return GetTaggedItems()->GetItemCount(std::string(tag));
}

const SessionItem* SessionItem::FindItem(const TagIndex& tag_index) const
{
  return GetTaggedItems()->FindItem(tag_index);
}

SessionItem* SessionItem::FindItem(const TagIndex& tag_index)
{
  return GetTaggedItems()->FindItem(tag_index);
}

std::vector<const SessionItem*> SessionItem::GetItems(std::string_view tag) const
{
  return GetTaggedItems()->GetItems(std::string(tag));
}

std::vector<SessionItem*> SessionItem::GetItems(std::string_view tag)
{
  return GetTaggedItems()->GetItems(std::string(tag));
}

std::vector<const SessionItem*> SessionItem::FindItemsImpl(std::string_view tag) const
{
  const auto* container = GetTaggedItems()->FindContainer(std::string(tag));
  return container != nullptr ? container->GetItems() : std::vector<const SessionItem*>{};
}

std::vector<SessionItem*> SessionItem::FindItemsImpl(std::string_view tag)
{
  auto* container = GetTaggedItems()->FindContainer(std::string(tag));
  return container != nullptr ? container->GetItems() : std::vector<SessionItem*>{};
}

TagIndex SessionItem::TagIndexOfItem(const SessionItem* item) const
{
  return GetTaggedItems()->TagIndexOfItem(item);
}

void SessionItem::RegisterTag(const TagInfo& tag_info, bool set_as_default)
{
  GetTaggedItems()->RegisterTag(tag_info, set_as_default);
}

SessionItem* SessionItem::InsertItem(std::unique_ptr<SessionItem> item, const TagIndex& tag_index)
{
  if (!item)
  {
    throw NullArgumentException("SessionItem::InsertItem: item is not specified");
  }

  auto insert_tag_index = utils::GetVerifiedTagIndex(*this, tag_index);
  utils::ValidateItemInsert(*item, *this, insert_tag_index);

  if (GetModel() != nullptr)
  {
    return ProcessCommand<InsertItemCommand>(*GetModel(), std::move(item), this, insert_tag_index);
  }

  return utils::InsertItemImpl(std::move(item), *this, insert_tag_index);
}

std::unique_ptr<SessionItem> SessionItem::TakeItem(const TagIndex& tag_index)
{
  utils::ValidateTakeItem(*this, tag_index);

  if (GetModel() != nullptr)
  {
    return ProcessCommand<RemoveItemCommand>(*GetModel(), this, tag_index);
  }

  return utils::TakeItemImpl(*this, tag_index);
}

bool SessionItem::IsEditable() const
{
  return !GetFlags(*this).IsSet(Appearance::kReadOnly);
}

SessionItem& SessionItem::SetEditable(bool value) &
{
  return SetFlag(Appearance::kReadOnly, !value);
}

bool SessionItem::IsEnabled() const
{
  return !GetFlags(*this).IsSet(Appearance::kDisabled);
}

SessionItem& SessionItem::SetEnabled(bool value) &
{
  return SetFlag(Appearance::kDisabled, !value);
}

bool SessionItem::IsVisible() const
{
  return !GetFlags(*this).IsSet(Appearance::kHidden);
}

SessionItem& SessionItem::SetVisible(bool value) &
{
  return SetFlag(Appearance::kHidden, !value);
}

std::string SessionItem::GetToolTip() const
{
  return HasData(DataRole::kTooltip) ? Data<std::string>(DataRole::kTooltip) : std::string();
}

SessionItem& SessionItem::SetToolTip(const std::string& tooltip) &
{
  (void)SetData(tooltip, DataRole::kTooltip);
  return *this;
}

std::string SessionItem::GetEditorType() const
{
  return HasData(DataRole::kEditor) ? Data<std::string>(DataRole::kEditor) : std::string();
}

SessionItem& SessionItem::SetEditorType(const std::string_view& editor_type) &
{
  (void)SetData(std::string(editor_type), DataRole::kEditor);
  return *this;
}

bool SessionItem::HasFlag(Appearance flag) const
{
  return GetFlags(*this).IsSet(flag);
}

SessionItem& SessionItem::SetFlag(Appearance flag, bool value) &
{
  SetFlags(GetFlags(*this).Set(flag, value), *this);
  return *this;
}

void SessionItem::SetModel(ISessionModel* model)
{
  p_impl->SetModel(model, this);
}

Slot* SessionItem::GetSlot() const
{
  return p_impl->GetSlot();
}

void SessionItem::SetParent(SessionItem* parent)
{
  p_impl->SetParent(parent);
}

SessionItemData* SessionItem::GetItemData()
{
  return p_impl->GetItemData();
}

const SessionItemData* SessionItem::GetItemData() const
{
  return p_impl->GetItemData();
}

const TaggedItems* SessionItem::GetTaggedItems() const
{
  return p_impl->GetTaggedItems();
}

TaggedItems* SessionItem::GetTaggedItems()
{
  return p_impl->GetTaggedItems();
}

void SessionItem::SetStrategy(const data_strategy_t& data_strategy)
{
  m_data_strategy = data_strategy;
}

void SessionItem::SetDataAndTags(const std::string& item_type,
                                 std::unique_ptr<SessionItemData> data,
                                 std::unique_ptr<TaggedItems> tags)
{
  p_impl = std::make_unique<SessionItemImpl>(item_type, std::move(data), std::move(tags));
}

}  // namespace mvvm
