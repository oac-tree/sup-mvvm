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

#include <mvvm/core/exceptions.h>
#include <mvvm/interfaces/item_factory_interface.h>
#include <mvvm/model/item_manager.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/tagged_items.h>
#include <mvvm/model/taginfo.h>
#include <mvvm/model/validate_utils.h>

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
      : m_self(self)
      , m_model_type(std::move(model_type))
      , m_item_manager(std::move(manager))
      , m_root_item(utils::CreateEmptyRootItem())
  {
  }

  //  //! Creates root item.
  //  void SetRootItem(std::unique_ptr<SessionItem> root_item)
  //  {
  //    m_root_item = std::move(root_item);

  //    // Root item can come from outside and can have a model already defined.
  //    // That means that the model decorator is handling this.

  //    if (!m_root_item->GetModel())
  //    {
  //      // If model is not defined, we have to set the model to ourself.
  //      m_root_item->SetModel(m_self);
  //    }
  //  }
};

SessionModel::SessionModel(std::string model_type)
    : SessionModel(model_type, CreateDefaultItemManager())
{
}

SessionModel::SessionModel(std::string model_type, std::unique_ptr<ItemManagerInterface> manager)
    : p_impl(std::make_unique<SessionModelImpl>(this, std::move(model_type), std::move(manager)))
{
  //  p_impl->SetRootItem(utils::CreateEmptyRootItem());
//  p_impl->m_root_item = utils::CreateEmptyRootItem();
  GetRootItem()->SetModel(this);
}

SessionModel::~SessionModel()
{
  // Explicitely call root item's destructor. It uses p_impl pointer during own descruction
  // and we have to keep pimpl pointer intact. Without line below will crash on MacOS because
  // of pecularities of MacOS libc++. See explanations here:
  // http://ibob.github.io/blog/2019/11/07/dont-use-unique_ptr-for-pimpl/
  p_impl->m_root_item.reset();
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

ModelEventSubscriberInterface* SessionModel::GetSubscriber() const
{
  return nullptr;
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

//! Removes item with given tag_index from the parent and returns it to the user.

std::unique_ptr<SessionItem> SessionModel::TakeItem(SessionItem* parent, const TagIndex& tag_index)
{
  return utils::TakeItem(*GetRootItem()->GetModel(), parent, tag_index);
}

//! Removes give item from the model.

void SessionModel::RemoveItem(SessionItem* item)
{
  utils::RemoveItem(*GetRootItem()->GetModel(), item);
}

//! Move item from it's current parent to a new parent under given tag and row.
//! Old and new parents should belong to this model.

void SessionModel::MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index)
{
  utils::MoveItem(*GetRootItem()->GetModel(), item, new_parent, tag_index);
}

//! Sets the data for given item.

bool SessionModel::SetData(SessionItem* item, const variant_t& value, int role)
{
  return item->SetData(value, role, /*direct*/ true);
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

void SessionModel::Clear(std::unique_ptr<SessionItem> root_item, SessionModelInterface *model)
{
  p_impl->m_root_item = root_item ? std::move(root_item) : utils::CreateEmptyRootItem();
//  if (!GetRootItem()->GetModel())
//  {
    GetRootItem()->SetModel(this);
//  }
  //  p_impl->SetRootItem(root_item ? std::move(root_item) : utils::CreateEmptyRootItem());
}

//! Registers item in pool. This will allow to find item pointer using its unique identifier.

void SessionModel::CheckIn(SessionItem* item)
{
  p_impl->m_item_manager->RegisterInPool(item);
}

//! Unregister item from pool.

void SessionModel::CheckOut(SessionItem* item)
{
  p_impl->m_item_manager->UnregisterFromPool(item);
}

}  // namespace mvvm
