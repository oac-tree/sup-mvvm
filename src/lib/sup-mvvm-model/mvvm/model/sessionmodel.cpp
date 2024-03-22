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

#include "sessionmodel.h"

#include "item_pool.h"
#include "model_composer.h"
#include "model_utils.h"
#include "sessionitem.h"
#include "validate_utils.h"

#include <mvvm/core/exceptions.h>

namespace
{
std::unique_ptr<mvvm::ModelComposerInterface> CreateDefaultComposer(
    mvvm::SessionModelInterface* model)
{
  return std::make_unique<mvvm::ModelComposer>(*model);
}

std::shared_ptr<mvvm::ItemPool> CreateDefaultItemPool()
{
  return std::make_shared<mvvm::ItemPool>();
}

}  // namespace

namespace mvvm
{

//! Pimpl class for SessionModel.

struct SessionModel::SessionModelImpl
{
  SessionModel* m_self{nullptr};
  std::string m_model_type;
  std::shared_ptr<ItemPool> m_pool;
  std::unique_ptr<ModelComposerInterface> m_composer;
  std::unique_ptr<SessionItem> m_root_item;

  SessionModelImpl(SessionModel* self, std::string model_type, std::shared_ptr<ItemPool> pool,
                   std::unique_ptr<ModelComposerInterface> composer)
      : m_self(self)
      , m_pool(pool ? std::move(pool) : CreateDefaultItemPool())
      , m_model_type(std::move(model_type))
      , m_composer(composer ? std::move(composer) : CreateDefaultComposer(m_self))
      , m_root_item(utils::CreateEmptyRootItem())
  {
  }
};

SessionModel::SessionModel(std::string model_type)
    : SessionModel(std::move(model_type), CreateDefaultItemPool(), CreateDefaultComposer(this))
{
}

SessionModel::SessionModel(std::string model_type, std::shared_ptr<ItemPool> pool,
                           std::unique_ptr<ModelComposerInterface> composer)
    : p_impl(std::make_unique<SessionModelImpl>(this, std::move(model_type), std::move(pool),
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

SessionItem* SessionModel::GetRootItem() const
{
  return p_impl->m_root_item.get();
}

ModelEventHandler* SessionModel::GetEventHandler() const
{
  // this implementation doesn't have any notification capabilities
  return nullptr;
}

SessionItem* SessionModel::InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                      const TagIndex& tag_index)
{
  if (!parent)
  {
    parent = GetRootItem();
  }

  auto actual_tagindex = utils::GetInsertTagIndex(parent, tag_index);
  utils::ValidateItemInsert(item.get(), parent, actual_tagindex);
  return p_impl->m_composer->InsertItem(std::move(item), parent, actual_tagindex);
}

std::unique_ptr<SessionItem> SessionModel::TakeItem(SessionItem* parent, const TagIndex& tag_index)
{
  utils::ValidateTakeItem(parent, tag_index);
  return p_impl->m_composer->TakeItem(parent, tag_index);
}

void SessionModel::RemoveItem(SessionItem* item)
{
  if (!item)
  {
    throw InvalidOperationException("Item is not initialised");
  }

  TakeItem(item->GetParent(), item->GetTagIndex());
}

void SessionModel::MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index)
{
  auto actual_tagindex = utils::GetInsertTagIndex(new_parent, tag_index);
  // FIXME move check to utils::ValidateItemMove
  if (item->GetParent() == new_parent && item->GetTagIndex() == actual_tagindex)
  {
    return;
  }

  utils::ValidateItemMove(item, new_parent, actual_tagindex);
  auto taken = TakeItem(item->GetParent(), item->GetTagIndex());
  InsertItem(std::move(taken), new_parent, tag_index);
}

bool SessionModel::SetData(SessionItem* item, const variant_t& value, int role)
{
  return p_impl->m_composer->SetData(item, value, role);
}

SessionItem* SessionModel::FindItem(const std::string& id) const
{
  return p_impl->m_pool->ItemForKey(id);
}

void SessionModel::Clear(std::unique_ptr<SessionItem> root_item)
{
  p_impl->m_composer->Reset(p_impl->m_root_item, std::move(root_item));
}

void SessionModel::CheckIn(SessionItem* item)
{
  return p_impl->m_pool->RegisterItem(item, item->GetIdentifier());
}

void SessionModel::CheckOut(SessionItem* item)
{
  return p_impl->m_pool->UnregisterItem(item);
}

void SessionModel::SetComposer(std::unique_ptr<ModelComposerInterface> composer)
{
  p_impl->m_composer = std::move(composer);
}

}  // namespace mvvm
