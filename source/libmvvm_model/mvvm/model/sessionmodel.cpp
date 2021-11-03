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

#include "mvvm/model/sessionmodel.h"

#include "mvvm/factories/itemcataloguefactory.h"
#include "mvvm/model/itemcatalogue.h"
#include "mvvm/model/itemfactory.h"
#include "mvvm/model/itemmanager.h"
#include "mvvm/model/itempool.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/taginfo.h"

namespace ModelView
{
//! Pimpl class for SessionModel.

struct SessionModel::SessionModelImpl
{
  SessionModel* m_self{nullptr};
  std::string m_model_type;
  std::unique_ptr<ItemManager> m_item_manager;
  std::unique_ptr<SessionItem> m_root_item;

  SessionModelImpl(SessionModel* self, std::string model_type, std::shared_ptr<ItemPool> pool)
      : m_self(self)
      , m_model_type(std::move(model_type))
      , m_item_manager(std::make_unique<ItemManager>())
  {
    m_item_manager->SetItemPool(pool ? std::move(pool) : std::make_shared<ItemPool>());
  }

  //! Creates root item.
  void CreateRootItem()
  {
    m_root_item = m_item_manager->CreateEmptyItem();
    m_root_item->SetModel(m_self);
    m_root_item->RegisterTag(TagInfo::CreateUniversalTag("rootTag"), /*set_as_default*/ true);
  }
};

//! Main c-tor.

SessionModel::SessionModel(std::string model_type, std::shared_ptr<ItemPool> pool)
    : p_impl(std::make_unique<SessionModelImpl>(this, std::move(model_type), std::move(pool)))

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

SessionItem* SessionModel::InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                      const TagIndex& tag_index)
{
  if (!parent)
  {
    parent = GetRootItem();
  }

  int actual_index = tag_index.index < 0 ? parent->GetItemCount(tag_index.tag) : tag_index.index;

  return parent->InsertItem(std::move(item), TagIndex{tag_index.tag, actual_index});
}

//! Insert new item using item's modelType.

SessionItem* SessionModel::InsertNewItem(const std::string& item_type, SessionItem* parent,
                                         const TagIndex& tag_index)
{
  return InsertItem(GetFactory()->CreateItem(item_type), parent, tag_index);
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
  if (!item || !item->GetModel() || !item->GetParent())
  {
    throw std::runtime_error("Invalid input item");
  }

  if (!new_parent || !new_parent->GetModel())
  {
    throw std::runtime_error("Invalid parent item");
  }

  if (item->GetModel() != new_parent->GetModel())
  {
    throw std::runtime_error("Items belong to different models");
  }

  auto taken = TakeItem(item->GetParent(), item->GetTagIndex());

  if (!taken)
  {
    throw std::runtime_error("Can't take an item");
  }

  if (!InsertItem(std::move(taken), new_parent, tag_index))
  {
    throw std::runtime_error("MoveItemCommand::execute() -> Can't insert item.");
  }
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

//! Sets brand new catalog of user-defined items. They become available for undo/redo and
//! serialization. Internally user catalog will be merged with the catalog of standard items.

void SessionModel::SetItemCatalogue(std::unique_ptr<ItemCatalogue> catalogue)
{
  AddStandardItemsToCatalogue(*catalogue);
  p_impl->m_item_manager->SetItemFactory(std::make_unique<ItemFactory>(std::move(catalogue)));
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

}  // namespace ModelView
