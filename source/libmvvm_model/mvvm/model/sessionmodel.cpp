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

#include "mvvm/model/sessionmodel.h"

#include "mvvm/core/exceptions.h"
#include "mvvm/interfaces/item_factory_interface.h"
#include "mvvm/model/item_manager.h"
#include "mvvm/model/model_utils.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/tagged_items.h"
#include "mvvm/model/taginfo.h"
#include "mvvm/model/validate_utils.h"

#include <sstream>

namespace mvvm
{
//! Pimpl class for SessionModel.

struct SessionModel::SessionModelImpl
{
  SessionModel* m_self{nullptr};
  std::string m_model_type;
  std::unique_ptr<ItemManagerInterface> m_item_manager;
  std::unique_ptr<SessionItem> m_root_item;

  SessionModelImpl(SessionModel* self, std::string model_type,
                   std::unique_ptr<ItemManagerInterface> manager)
      : m_self(self), m_model_type(std::move(model_type)), m_item_manager(std::move(manager))
  {
  }

  //! Creates root item.
  void CreateRootItem()
  {
    m_root_item = m_item_manager->CreateItem(SessionItem::Type);
    m_root_item->SetModel(m_self);
    m_root_item->RegisterTag(TagInfo::CreateUniversalTag("rootTag"), /*set_as_default*/ true);
  }
};

SessionModel::SessionModel(std::string model_type)
    : SessionModel(model_type, CreateDefaultItemManager())
{
}

SessionModel::SessionModel(std::string model_type, std::unique_ptr<ItemManagerInterface> manager)
    : p_impl(std::make_unique<SessionModelImpl>(this, std::move(model_type), std::move(manager)))
{
  p_impl->CreateRootItem();
}

SessionModel::~SessionModel()
{
  // Explicitely call root item's destructor. It uses p_impl pointer during own descruction
  // and we have to keep pimpl pointer intact. Without line below will crash on MacOS because
  // of pecularities of MacOS libc++. See explanations here:
  // http://ibob.github.io/blog/2019/11/07/dont-use-unique_ptr-for-pimpl/
  p_impl->m_root_item.reset();
}

//! Insert item via move into the given `parent` under given `tag_index`.
//! FIXME make default parameters (or their absence) as in the method InsertNewItem.

SessionItem* SessionModel::InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                      const TagIndex& tag_index)
{
  if (!parent)
  {
    parent = GetRootItem();
  }

  auto actual_tagindex = utils::GetActualInsertTagIndex(parent, tag_index);
  utils::ValidateItemInsert(item.get(), parent, actual_tagindex);

  return parent->InsertItem(std::move(item), actual_tagindex);
}

//! Insert new item corresponding to the given `item_type` into the `parent` under given
//! `tag_index`.

SessionItem* SessionModel::InsertNewItem(const std::string& item_type, SessionItem* parent,
                                         const TagIndex& tag_index)
{
  return InsertItem(p_impl->m_item_manager->CreateItem(item_type), parent, tag_index);
}

//! Removes item with given tag_index from the parent and returns it to the user.

std::unique_ptr<SessionItem> SessionModel::TakeItem(SessionItem* parent, const TagIndex& tag_index)
{
  if (!parent)
  {
    throw std::runtime_error("Error in SessionModel::removeItem(): parent is undefined");
  }
  if (parent->GetModel() != this)
  {
    throw std::runtime_error(
        "Error in SessionModel::removeItem(): item doesn't belong to given model");
  }
  return parent->TakeItem(tag_index);
}

//! Removes give item from the model.

void SessionModel::RemoveItem(SessionItem* item)
{
  if (!item)
  {
    throw std::runtime_error("Item is not initialised");
  }
  TakeItem(item->GetParent(), item->GetTagIndex());
}

//! Move item from it's current parent to a new parent under given tag and row.
//! Old and new parents should belong to this model.

void SessionModel::MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index)
{
  utils::ValidateItemMove(item, new_parent, tag_index);

  auto taken = TakeItem(item->GetParent(), item->GetTagIndex());
  InsertItem(std::move(taken), new_parent, tag_index);
}

//! Returns the data for given item and role.

variant_t SessionModel::Data(SessionItem* item, int role)
{
  return item->Data(role);
}

//! Sets the data for given item.

bool SessionModel::SetData(SessionItem* item, const variant_t& value, int role)
{
  return item->SetData(value, role, /*direct*/ true);
}

//! Returns model type.

std::string SessionModel::GetType() const
{
  return p_impl->m_model_type;
}

//! Returns root item of the model.

SessionItem* SessionModel::GetRootItem() const
{
  return p_impl->m_root_item.get();
}

//! Returns item factory which can generate all items supported by this model.

const ItemFactoryInterface* SessionModel::GetFactory() const
{
  return p_impl->m_item_manager->GetFactory();
}

//! Returns SessionItem for given identifier.

SessionItem* SessionModel::FindItem(const std::string& id) const
{
  return p_impl->m_item_manager->FindItem(id);
}

//! Removes all items from the model. If callback is provided, use it to rebuild content of root
//! item (used while restoring the model from serialized content).

void SessionModel::Clear(std::function<void(SessionItem*)> callback)
{
  p_impl->CreateRootItem();
  if (callback)
  {
    callback(GetRootItem());
  }
}

//! Registers item in pool. This will allow to find item pointer using its unique identifier.

void SessionModel::RegisterInPool(SessionItem* item)
{
  p_impl->m_item_manager->RegisterInPool(item);
  if (utils::HasSignals(this))
  {
    item->Activate();
  }
}

//! Unregister item from pool.

void SessionModel::UnregisterFromPool(SessionItem* item)
{
  p_impl->m_item_manager->UnregisterFromPool(item);
}

void SessionModel::RegisterItemInternal(const std::string& item_type,
                                        const item_factory_func_t& func, const std::string& label)
{
  p_impl->m_item_manager->GetFactory()->RegisterItem(item_type, func, label);
}

}  // namespace mvvm
