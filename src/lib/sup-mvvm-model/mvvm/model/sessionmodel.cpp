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

#include "sessionmodel.h"

#include "item_manager.h"
#include "model_composer.h"
#include "model_utils.h"
#include "sessionitem.h"
#include "validate_utils.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/interfaces/item_factory_interface.h>

#include <sstream>

namespace
{
std::unique_ptr<mvvm::ModelComposerInterface> CreateDefaultComposer(
    mvvm::SessionModelInterface* model)
{
  return std::make_unique<mvvm::ModelComposer>(*model);
}

}  // namespace

namespace mvvm
{
//! Pimpl class for SessionModel.

struct SessionModel::SessionModelImpl
{
  SessionModel* m_self{nullptr};
  std::string m_model_type;
  std::unique_ptr<ItemManagerInterface> m_item_manager;
  std::unique_ptr<ModelComposerInterface> m_composer;
  std::unique_ptr<SessionItem> m_root_item;

  SessionModelImpl(SessionModel* self, std::string model_type,
                   std::unique_ptr<ItemManagerInterface> manager,
                   std::unique_ptr<ModelComposerInterface> composer)
      : m_self(self)
      , m_model_type(std::move(model_type))
      , m_item_manager(std::move(manager))
      , m_composer(composer ? std::move(composer) : CreateDefaultComposer(m_self))
      , m_root_item(utils::CreateEmptyRootItem())
  {
  }
};

SessionModel::SessionModel(std::string model_type)
    : SessionModel(model_type, CreateDefaultItemManager(), CreateDefaultComposer(this))
{
}

SessionModel::SessionModel(std::string model_type, std::unique_ptr<ItemManagerInterface> manager,
                           std::unique_ptr<ModelComposerInterface> composer)
    : p_impl(std::make_unique<SessionModelImpl>(this, std::move(model_type), std::move(manager),
                                                std::move(composer)))
{
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

ModelEventHandler* SessionModel::GetEventHandler() const
{
  return nullptr;
}

//! Insert item via move into the given `parent` under given `tag_index`.

SessionItem* SessionModel::InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                      const TagIndex& tag_index)
{
  if (!parent)
  {
    parent = GetRootItem();
  }

  auto actual_tagindex = utils::GetActualInsertTagIndex(parent, tag_index);
  utils::ValidateItemInsert(item.get(), parent, actual_tagindex);

  return p_impl->m_composer->InsertItem(std::move(item), parent, actual_tagindex);
}

//! Removes item with given tag_index from the parent and returns it to the user.

std::unique_ptr<SessionItem> SessionModel::TakeItem(SessionItem* parent, const TagIndex& tag_index)
{
  utils::ValidateTakeItem(this, parent, tag_index);
  return p_impl->m_composer->TakeItem(parent, tag_index);
}

//! Removes give item from the model.

void SessionModel::RemoveItem(SessionItem* item)
{
  if (!item)
  {
    throw InvalidOperationException("Item is not initialised");
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

//! Sets the data for given item.

bool SessionModel::SetData(SessionItem* item, const variant_t& value, int role)
{
  return p_impl->m_composer->SetData(item, value, role);
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

void SessionModel::Clear(std::unique_ptr<SessionItem> root_item)
{
  p_impl->m_composer->Reset(p_impl->m_root_item, std::move(root_item));
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

void SessionModel::SetComposer(std::unique_ptr<ModelComposerInterface> composer)
{
  p_impl->m_composer = std::move(composer);
}

}  // namespace mvvm
