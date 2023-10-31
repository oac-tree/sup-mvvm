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

#include "mvvm/model/sessionitem.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/core/unique_id_generator.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/sessionitem_data.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/model/taginfo.h>

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
  SessionModelInterface* m_model{nullptr};
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

//! Parameterized copy constructor to allow item clone and deep item copy.
//! If \it make_unique_id is true, identifiers of the item and all its children will be regenerated.
//! This will make an item unique and will allow its usage (serialization, memory pool) along with
//! the original. If \it make_unique_id is false, the result will be an exact clone of the original.

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

//! Creates clone of the item. If \it make_unique_id is true (the default case),
//! identifiers of the item and all its children will be regenerated.

std::unique_ptr<SessionItem> SessionItem::Clone(bool make_unique_id) const
{
  return std::make_unique<SessionItem>(*this, make_unique_id);
}

//! Returns item's model type.

std::string SessionItem::GetType() const
{
  return p_impl->m_item_type;
}

//! Returns unique identifier.

std::string SessionItem::GetIdentifier() const
{
  return Data<std::string>(DataRole::kIdentifier);
}

//! Sets display name (fluent interface).

SessionItem& SessionItem::SetDisplayName(const std::string& name)
{
  SetData(name, DataRole::kDisplay);
  return *this;
}

//! Returns display name.

std::string SessionItem::GetDisplayName() const
{
  return HasData(DataRole::kDisplay) ? Data<std::string>(DataRole::kDisplay) : GetType();
}

//! Returns the model to which given item belongs to. Will return nullptr if item doesn't have a
//! model.

SessionModelInterface* SessionItem::GetModel() const
{
  return p_impl->m_model;
}

//! Returns parent item. Will return nullptr if item doesn't have a parent.

SessionItem* SessionItem::GetParent() const
{
  return p_impl->m_parent;
}

//! Returns TagIndex of this item under which it is accessible through its parent.

TagIndex SessionItem::GetTagIndex() const
{
  return GetParent() ? GetParent()->TagIndexOfItem(this) : TagIndex();
}

//! Returns true if item has data on board with given role.

bool SessionItem::HasData(int role) const
{
  return p_impl->m_data->HasData(role);
}

//! Specialized version for const char: method is introduced to avoid "const char" conversion into
//! variant<bool>.
bool SessionItem::SetData(const char* value, int role, bool direct)
{
  return SetData(std::string(value), role, direct);
}

//! Returns pointer to item's data container (const version).

const SessionItemData* SessionItem::GetItemData() const
{
  return p_impl->m_data.get();
}

//! Returns pointer to item's data container (non-const version).

SessionItemData* SessionItem::GetItemData()
{
  return const_cast<SessionItemData*>(static_cast<const SessionItem*>(this)->GetItemData());
}

//! Returns total number of children in all tags.

int SessionItem::GetTotalItemCount() const
{
  return static_cast<int>(GetAllItems().size());
}

//! Returns vector of children formed from all chidlren from all tags.

std::vector<SessionItem*> SessionItem::GetAllItems() const
{
  return p_impl->m_tags->GetAllItems();
}

//! Returns number of items in given tag.

int SessionItem::GetItemCount(const std::string& tag) const
{
  return p_impl->m_tags->GetItemCount(tag);
}

//! Returns item located at given \it tag_index.
//! Will throw if \it tag_index is invalid (wrong index, non-existing container).

SessionItem* SessionItem::GetItem(const TagIndex& tag_index) const
{
  return p_impl->m_tags->GetItem(tag_index);
}

//! Returns all children stored at given tag.

std::vector<SessionItem*> SessionItem::GetItems(const std::string& tag) const
{
  return p_impl->m_tags->GetItems(tag);
}

//! Returns pair of tag and index corresponding to given item.
//! Returns {"", -1} if given item doesn't belong to children.

TagIndex SessionItem::TagIndexOfItem(const SessionItem* item) const
{
  return p_impl->m_tags->TagIndexOfItem(item);
}

//! Returns pointer to internal collection of tag-registered items (const version).

const TaggedItems* SessionItem::GetTaggedItems() const
{
  return p_impl->m_tags.get();
}

//! Registers tag to hold items under given name.

void SessionItem::RegisterTag(const TagInfo& tagInfo, bool set_as_default)
{
  p_impl->m_tags->RegisterTag(tagInfo, set_as_default);
}

//! Returns pointer to internal collection of tag-registered items (non-const version).

TaggedItems* SessionItem::GetTaggedItems()
{
  return const_cast<TaggedItems*>(static_cast<const SessionItem*>(this)->GetTaggedItems());
}

//! Insert item into given tag under the given index. Will take ownership of inserted item.
//! Returns back a pointer to the same item for convenience.

SessionItem* SessionItem::InsertItem(std::unique_ptr<SessionItem> item, const TagIndex& tag_index)
{
  if (!item)
  {
    throw InvalidOperationException("Invalid item");
  }

  if (item.get() == this)
  {
    throw InvalidOperationException("Attempt to insert to itself");
  }

  if (item->GetParent())
  {
    throw InvalidOperationException("Item already has a parent");
  }

  if (item->GetModel())
  {
    throw InvalidOperationException("Item already has a model");
  }

  if (utils::IsItemAncestor(this, item.get()))
  {
    throw InvalidOperationException("Attempt to turn ancestor into a child");
  }

  if (!p_impl->m_tags->CanInsertItem(item.get(), tag_index))
  {
    throw InvalidOperationException("Wrong tags");
  }

  auto result = p_impl->m_tags->InsertItem(std::move(item), tag_index);
  result->SetParent(this);
  result->SetModel(GetModel());

  return result;
}

//! Removes item from given index from given tag, returns it to the caller.
//! Ownership is granted to the caller.

std::unique_ptr<SessionItem> SessionItem::TakeItem(const TagIndex& tag_index)
{
  if (!p_impl->m_tags->CanTakeItem(tag_index))
  {
    return {};
  }

  auto result = p_impl->m_tags->TakeItem(tag_index);
  result->SetParent(nullptr);
  result->SetModel(nullptr);

  return std::move(result);
}

//! Returns true if this item has `editable` flag set.
//! The data value of an editable item normally can be changed when it appears in trees and tables.

bool SessionItem::IsEditable() const
{
  return !(appearance(*this) & Appearance::kReadOnly);
}

//! Sets `editable` flag to given value (fluent interface).

SessionItem& SessionItem::SetEditable(bool value)
{
  return SetFlag(Appearance::kReadOnly, !value);
}

//! Returns true if this item has `enabled` flag set.

bool SessionItem::IsEnabled() const
{
  return !(appearance(*this) & Appearance::kDisabled);
}

//! Sets `enabled` flag to given value (fluent interface). Used in Qt-widgets to show that given
//! property is currently enabled. Enabled items appear in normal color, disabled items are grayed
//! out.

SessionItem& SessionItem::SetEnabled(bool value)
{
  return SetFlag(Appearance::kDisabled, !value);
}

//! Returns true if this item has `visible` flag set.

bool SessionItem::IsVisible() const
{
  return !(appearance(*this) & Appearance::kHidden);
}

//! Sets `visible` flag to given value (fluent interface). Used in Qt-widgets to hide given
//! property from a view. For example, `PropertyTreeView` will not show PropertyItem with the given
//! flag set to `true` among other properties.

SessionItem& SessionItem::SetVisible(bool value)
{
  return SetFlag(Appearance::kHidden, !value);
}

//! Returns item tooltip, if exists.

std::string SessionItem::GetToolTip() const
{
  return HasData(DataRole::kTooltip) ? Data<std::string>(DataRole::kTooltip) : std::string();
}

//! Sets item tooltip (fluent interface).

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

//! Sets appearance flag to given value.

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

//! Activates business logic.
//! The method is called from the model when on item insertion.

void SessionItem::Activate() {}

void SessionItem::SetModel(SessionModelInterface* model)
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

//! Sets the data for given role. Method invented to hide implementaiton details.

bool SessionItem::SetDataInternal(const variant_t& value, int role, bool direct)
{
  const bool act_through_model = !direct && GetModel();
  return act_through_model ? GetModel()->SetData(this, value, role)
                           : p_impl->m_data->SetData(value, role);
}

//! Returns data for given role. Method invented to hide implementaiton details and avoid
//! placing sessionitem_data.h into 'sessionitem.h' header.

variant_t SessionItem::DataInternal(int role) const
{
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
