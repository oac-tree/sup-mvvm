/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sessionitem.h"

#include "sessionitem_data.h"
#include "tagged_items.h"
#include "taginfo.h"
#include "validate_utils.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/core/unique_id_generator.h>
#include <mvvm/interfaces/sessionmodel_interface.h>

namespace
{
int appearance(const mvvm::SessionItem& item)
{
  return item.HasData(mvvm::DataRole::kAppearance) ? item.Data<int>(mvvm::DataRole::kAppearance)
                                                   : mvvm::Appearance::kDefault;
}
}  // namespace

namespace mvvm
{
struct SessionItem::SessionItemImpl
{
  SessionItem* m_parent{nullptr};
  ISessionModel* m_model{nullptr};
  std::unique_ptr<SessionItemData> m_data;
  std::unique_ptr<TaggedItems> m_tags;
  std::string m_item_type;
};

SessionItem::SessionItem() : SessionItem(Type) {}

SessionItem::SessionItem(const std::string& item_type)
    : SessionItem(item_type, std::make_unique<SessionItemData>(), std::make_unique<TaggedItems>())
{
  SetData(UniqueIdGenerator::Generate(), DataRole::kIdentifier);
}

SessionItem::SessionItem(const std::string& item_type, std::unique_ptr<SessionItemData> data,
                         std::unique_ptr<TaggedItems> tags)
    : p_impl(std::make_unique<SessionItemImpl>())
{
  p_impl->m_data = std::move(data);
  p_impl->m_tags = std::move(tags);
  p_impl->m_item_type = item_type;
}

SessionItem::~SessionItem()
{
  if (p_impl->m_model)
  {
    p_impl->m_model->CheckOut(this);
  }
}

SessionItem::SessionItem(const SessionItem& other, bool make_unique_id)
    : SessionItem(other.GetType(), std::make_unique<SessionItemData>(*other.GetItemData()),
                  other.GetTaggedItems()->Clone(make_unique_id))
{
  if (make_unique_id)
  {
    SetData(UniqueIdGenerator::Generate(), DataRole::kIdentifier);
  }
  for (auto child : GetAllItems())
  {
    child->SetParent(this);
  }
}

std::unique_ptr<SessionItem> SessionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<SessionItem>(*this, make_unique_id);
}

std::string SessionItem::GetType() const
{
  return p_impl->m_item_type;
}

std::string SessionItem::GetIdentifier() const
{
  return Data<std::string>(DataRole::kIdentifier);
}

std::string SessionItem::GetDisplayName() const
{
  return HasData(DataRole::kDisplay) ? Data<std::string>(DataRole::kDisplay) : GetType();
}

SessionItem& SessionItem::SetDisplayName(const std::string& name)
{
  SetData(name, DataRole::kDisplay);
  return *this;
}

ISessionModel* SessionItem::GetModel() const
{
  return p_impl->m_model;
}

SessionItem* SessionItem::GetParent() const
{
  return p_impl->m_parent;
}

TagIndex SessionItem::GetTagIndex() const
{
  return GetParent() ? GetParent()->TagIndexOfItem(this) : TagIndex();
}

bool SessionItem::HasData(int role) const
{
  return p_impl->m_data->HasData(role);
}

bool SessionItem::SetData(const char* value, int role, bool direct)
{
  return SetData(std::string(value), role, direct);
}

SessionItemData* SessionItem::GetItemData()
{
  return const_cast<SessionItemData*>(static_cast<const SessionItem*>(this)->GetItemData());
}

const SessionItemData* SessionItem::GetItemData() const
{
  return p_impl->m_data.get();
}

int SessionItem::GetTotalItemCount() const
{
  return static_cast<int>(GetAllItems().size());
}

std::vector<SessionItem*> SessionItem::GetAllItems() const
{
  return p_impl->m_tags->GetAllItems();
}

int SessionItem::GetItemCount(const std::string& tag) const
{
  return p_impl->m_tags->GetItemCount(tag);
}

SessionItem* SessionItem::GetItem(const TagIndex& tag_index) const
{
  return p_impl->m_tags->GetItem(tag_index);
}

std::vector<SessionItem*> SessionItem::GetItems(const std::string& tag) const
{
  return p_impl->m_tags->GetItems(tag);
}

TagIndex SessionItem::TagIndexOfItem(const SessionItem* item) const
{
  return p_impl->m_tags->TagIndexOfItem(item);
}

void SessionItem::RegisterTag(const TagInfo& tag_info, bool set_as_default)
{
  p_impl->m_tags->RegisterTag(tag_info, set_as_default);
}

const TaggedItems* SessionItem::GetTaggedItems() const
{
  return p_impl->m_tags.get();
}

TaggedItems* SessionItem::GetTaggedItems()
{
  return const_cast<TaggedItems*>(static_cast<const SessionItem*>(this)->GetTaggedItems());
}

SessionItem* SessionItem::InsertItem(std::unique_ptr<SessionItem> item, const TagIndex& tag_index)
{
  auto insert_tag_index = p_impl->m_tags->GetInsertTagIndex(tag_index);
  utils::ValidateItemInsert(item.get(), this, insert_tag_index);

  auto result = p_impl->m_tags->InsertItem(std::move(item), insert_tag_index);
  result->SetParent(this);
  result->SetModel(GetModel());
  return result;
}

std::unique_ptr<SessionItem> SessionItem::TakeItem(const TagIndex& tag_index)
{
  utils::ValidateTakeItem(this, tag_index);

  auto result = p_impl->m_tags->TakeItem(tag_index);
  result->SetParent(nullptr);
  result->SetModel(nullptr);

  return std::move(result);
}

bool SessionItem::IsEditable() const
{
  return !(appearance(*this) & Appearance::kReadOnly);
}

SessionItem& SessionItem::SetEditable(bool value)
{
  return SetFlag(Appearance::kReadOnly, !value);
}

bool SessionItem::IsEnabled() const
{
  return !(appearance(*this) & Appearance::kDisabled);
}

SessionItem& SessionItem::SetEnabled(bool value)
{
  return SetFlag(Appearance::kDisabled, !value);
}

bool SessionItem::IsVisible() const
{
  return !(appearance(*this) & Appearance::kHidden);
}

SessionItem& SessionItem::SetVisible(bool value)
{
  return SetFlag(Appearance::kHidden, !value);
}

std::string SessionItem::GetToolTip() const
{
  return HasData(DataRole::kTooltip) ? Data<std::string>(DataRole::kTooltip) : std::string();
}

SessionItem& SessionItem::SetToolTip(const std::string& tooltip)
{
  SetData(tooltip, DataRole::kTooltip);
  return *this;
}

std::string SessionItem::GetEditorType() const
{
  return HasData(DataRole::kEditor) ? Data<std::string>(DataRole::kEditor) : std::string();
}

SessionItem& SessionItem::SetEditorType(const std::string& editor_type)
{
  SetData(editor_type, DataRole::kEditor);
  return *this;
}

bool SessionItem::HasFlag(Appearance flag) const
{
  return HasData(DataRole::kAppearance) ? Data<int>(DataRole::kAppearance) & flag : false;
}

SessionItem& SessionItem::SetFlag(Appearance flag, bool value)
{
  int flags = appearance(*this);
  if (value)
  {
    flags |= flag;
  }
  else
  {
    flags &= ~flag;
  }

  SetData(flags, DataRole::kAppearance);
  return *this;
}

void SessionItem::Activate() {}

void SessionItem::SetModel(ISessionModel* model)
{
  if (p_impl->m_model)
  {
    p_impl->m_model->CheckOut(this);
  }

  p_impl->m_model = model;

  if (p_impl->m_model)
  {
    p_impl->m_model->CheckIn(this);
  }

  for (auto child : GetAllItems())
  {
    child->SetModel(model);
  }
}

bool SessionItem::SetDataInternal(const variant_t& value, int role, bool direct)
{
  // Method invented to hide implementaiton details and avoid placing SessionItemData header into
  // SessionItem header.
  const bool act_through_model = !direct && GetModel();
  return act_through_model ? GetModel()->SetData(this, value, role)
                           : p_impl->m_data->SetData(value, role);
}

variant_t SessionItem::DataInternal(int role) const
{
  // Method invented to hide implementaiton details and avoid placing SessionItemData header into
  // SessionItem header.
  return p_impl->m_data->Data(role);
}

void SessionItem::SetParent(SessionItem* parent)
{
  p_impl->m_parent = parent;
}

void SessionItem::SetDataAndTags(std::unique_ptr<SessionItemData> data,
                                 std::unique_ptr<TaggedItems> tags)
{
  p_impl->m_data = std::move(data);
  p_impl->m_tags = std::move(tags);
}

}  // namespace mvvm
