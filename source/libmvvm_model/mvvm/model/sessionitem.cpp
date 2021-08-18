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

#include "mvvm/model/sessionitem.h"

#include "mvvm/core/uniqueidgenerator.h"
#include "mvvm/model/sessionitemdata.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/model/taggeditems.h"
#include "mvvm/model/taginfo.h"

#include <stdexcept>

using namespace ModelView;

namespace
{
int appearance(const ModelView::SessionItem& item)
{
  const int default_appearance =
      Appearance::kEditable | Appearance::kEnabled | Appearance::kVisible;
  return item.HasData(DataRole::kAppearance) ? item.Data<int>(DataRole::kAppearance)
                                             : default_appearance;
}
}  // namespace

struct SessionItem::SessionItemImpl
{
  SessionItem* m_self{nullptr};
  SessionItem* m_parent{nullptr};
  SessionModel* m_model{nullptr};
  std::unique_ptr<SessionItemData> m_data;
  std::unique_ptr<TaggedItems> m_tags;
  std::string m_item_type;

  SessionItemImpl(SessionItem* this_item)
      : m_self(this_item)
      , m_data(std::make_unique<SessionItemData>())
      , m_tags(std::make_unique<TaggedItems>())
  {
  }
};

SessionItem::SessionItem() : SessionItem(Type) {}

SessionItem::SessionItem(const std::string& item_type)
    : p_impl(std::make_unique<SessionItemImpl>(this))
{
  p_impl->m_item_type = item_type;
  SetData(UniqueIdGenerator::Generate(), DataRole::kIdentifier);
  SetData(p_impl->m_item_type, DataRole::kDisplay);
}

SessionItem::~SessionItem()
{
  if (p_impl->m_model)
    p_impl->m_model->unregisterFromPool(this);
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

SessionItem* SessionItem::SetDisplayName(const std::string& name)
{
  SetData(name, DataRole::kDisplay);
  return this;
}

//! Returns display name.

std::string SessionItem::GetDisplayName() const
{
  return Data<std::string>(DataRole::kDisplay);
}

//! Returns the model to which given item belongs to. Will return nullptr if item doesn't have a
//! model.

SessionModel* SessionItem::GetModel() const
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

//! Returns pointer to item's data container (const version).

const SessionItemData* SessionItem::itemData() const
{
  return p_impl->m_data.get();
}

//! Returns pointer to item's data container (non-const version).

SessionItemData* SessionItem::itemData()
{
  return const_cast<SessionItemData*>(static_cast<const SessionItem*>(this)->itemData());
}

//! Returns total number of children in all tags.

int SessionItem::childrenCount() const
{
  return static_cast<int>(children().size());
}

//! Returns vector of children formed from all chidlren from all tags.

std::vector<SessionItem*> SessionItem::children() const
{
  return p_impl->m_tags->GetAllItems();
}

//! Returns number of items in given tag.

int SessionItem::GetItemCount(const std::string& tag) const
{
  return p_impl->m_tags->GetItemCount(tag);
}

//! Returns item at given row of given tag.
//! Will throw if container with such `tag` doesn't exist, or `index` is invalid.

SessionItem* SessionItem::getItem(const std::string& tag, int index) const
{
  return p_impl->m_tags->GetItem({tag, index});
}

//! Returns all children stored at given tag.

std::vector<SessionItem*> SessionItem::getItems(const std::string& tag) const
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

const TaggedItems* SessionItem::itemTags() const
{
  return p_impl->m_tags.get();
}

//! Registers tag to hold items under given name.

void SessionItem::RegisterTag(const TagInfo& tagInfo, bool set_as_default)
{
  p_impl->m_tags->RegisterTag(tagInfo, set_as_default);
}

//! Returns pointer to internal collection of tag-registered items (non-const version).

TaggedItems* SessionItem::itemTags()
{
  return const_cast<TaggedItems*>(static_cast<const SessionItem*>(this)->itemTags());
}

//! Insert item into given tag under the given index. Will take ownership of inserted item.
//! Returns back a pointer to the same item for convenience.

SessionItem* SessionItem::InsertItem(std::unique_ptr<SessionItem> item, const TagIndex& tag_index)
{
  if (!item)
    throw std::runtime_error("SessionItem::insertItem() -> Invalid item.");

  if (item->GetParent())
    throw std::runtime_error("SessionItem::insertItem() -> Existing parent.");

  if (item->GetModel())
    throw std::runtime_error("SessionItem::insertItem() -> Existing model.");

  if (!p_impl->m_tags->CanInsertItem(item.get(), tag_index))
    throw std::runtime_error("SessionItem::insertItem() -> Can't insert item.");

  auto result = item.release();
  p_impl->m_tags->InsertItem(result, tag_index);
  result->setParent(this);
  result->setModel(GetModel());

  return result;
}

//! Removes item from given index from given tag, returns it to the caller.
//! Ownership is granted to the caller.

std::unique_ptr<SessionItem> SessionItem::TakeItem(const TagIndex& tag_index)
{
  if (!p_impl->m_tags->CanTakeItem(tag_index))
    return {};

  auto result = p_impl->m_tags->TakeItem(tag_index);
  result->setParent(nullptr);
  result->setModel(nullptr);

  return std::unique_ptr<SessionItem>(result);
}

//! Returns true if this item has `editable` flag set.
//! The data value of an editable item normally can be changed when it appears in trees and tables.

bool SessionItem::IsEditable() const
{
  return appearance(*this) & Appearance::kEditable;
}

//! Sets `editable` flag to given value (fluent interface).

SessionItem* SessionItem::SetEditable(bool value)
{
  setAppearanceFlag(Appearance::kEditable, value);
  return this;
}

//! Returns true if this item has `enabled` flag set.

bool SessionItem::IsEnabled() const
{
  return appearance(*this) & Appearance::kEnabled;
}

//! Sets `enabled` flag to given value (fluent interface). Used in Qt-widgets to show that given
//! property is currently enabled. Enabled items appear in normal color, disabled items are grayed
//! out.

SessionItem* SessionItem::SetEnabled(bool value)
{
  setAppearanceFlag(Appearance::kEnabled, value);
  return this;
}

//! Returns true if this item has `visible` flag set.

bool SessionItem::IsVisible() const
{
  return appearance(*this) & Appearance::kVisible;
}

//! Sets `visible` flag to given value (fluent interface). Used in Qt-widgets to hide given
//! property from a view. For example, `PropertyTreeView` will not show PropertyItem with the given
//! flag set to `true` among other properties.

SessionItem* SessionItem::SetVisible(bool value)
{
  setAppearanceFlag(Appearance::kVisible, value);
  return this;
}

//! Returns item tooltip, if exists.

std::string SessionItem::GetToolTip() const
{
  return HasData(DataRole::kTooltip) ? Data<std::string>(DataRole::kTooltip) : std::string();
}

//! Sets item tooltip (fluent interface).

SessionItem* SessionItem::SetToolTip(const std::string& tooltip)
{
  SetData(tooltip, DataRole::kTooltip);
  return this;
}

//! Sets the data for given role. Method invented to hide implementaiton details.

bool SessionItem::set_data_internal(const variant_t& value, int role)
{
  return p_impl->m_data->SetData(value, role);
}

//! Returns data for given role. Method invented to hide implementaiton details and avoid
//! placing sessionitemdata.h into 'sessionitem.h' header.

variant_t SessionItem::data_internal(int role) const
{
  return p_impl->m_data->Data(role);
}

void SessionItem::setParent(SessionItem* parent)
{
  p_impl->m_parent = parent;
}

void SessionItem::setModel(SessionModel* model)
{
  if (p_impl->m_model)
    p_impl->m_model->unregisterFromPool(this);

  p_impl->m_model = model;

  if (p_impl->m_model)
    p_impl->m_model->registerInPool(this);

  for (auto child : children())
    child->setModel(model);
}

void SessionItem::setAppearanceFlag(int flag, bool value)
{
  int flags = appearance(*this);
  if (value)
    flags |= flag;
  else
    flags &= ~flag;

  SetData(flags, DataRole::kAppearance);
}

void SessionItem::setDataAndTags(std::unique_ptr<SessionItemData> data,
                                 std::unique_ptr<TaggedItems> tags)
{
  p_impl->m_data = std::move(data);
  p_impl->m_tags = std::move(tags);
}
