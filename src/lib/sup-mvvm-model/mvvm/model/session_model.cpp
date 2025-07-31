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

#include "session_model.h"

#include "item_pool.h"
#include "item_utils.h"
#include "model_utils.h"
#include "session_item.h"
#include "validate_utils.h"

#include <mvvm/commands/default_command_processor.h>
#include <mvvm/commands/i_command.h>
#include <mvvm/core/mvvm_exceptions.h>

#include <sstream>

namespace
{

std::shared_ptr<mvvm::ItemPool> CreateDefaultItemPool()
{
  return std::make_shared<mvvm::ItemPool>();
}

std::unique_ptr<mvvm::ICommandStack> CreateDefaultCommandProcessor()
{
  return std::make_unique<mvvm::DefaultCommandProcessor>();
}

}  // namespace

namespace mvvm
{

/**
 * @brief Pimpl class for SessionModel.
 */
struct SessionModel::SessionModelImpl
{
  std::string m_model_type;
  std::shared_ptr<ItemPool> m_pool;
  std::unique_ptr<ICommandStack> m_command_processor;
  std::unique_ptr<SessionItem> m_root_item;
  bool m_is_executing{false};

  SessionModelImpl(std::string model_type, std::shared_ptr<ItemPool> pool,
                   std::unique_ptr<ICommandStack> command_processor)
      : m_model_type(std::move(model_type))
      , m_pool(pool ? std::move(pool) : CreateDefaultItemPool())
      , m_command_processor(command_processor ? std::move(command_processor)
                                              : CreateDefaultCommandProcessor())
      , m_root_item(utils::CreateEmptyRootItem())
  {
  }
};

SessionModel::SessionModel(std::string model_type)
    : SessionModel(std::move(model_type), CreateDefaultItemPool())
{
}

SessionModel::SessionModel(std::string model_type, std::shared_ptr<ItemPool> pool,
                           std::unique_ptr<ICommandStack> command_stack)
    : p_impl(std::make_unique<SessionModelImpl>(std::move(model_type), std::move(pool),
                                                std::move(command_stack)))
{
  GetRootItem()->SetModel(this);
}

SessionModel::SessionModel(const SessionModel& other)
    : SessionModel(other.GetType(), {}, other.p_impl->m_command_processor->Clone())
{
  ReplaceRootItem(utils::CopyItem(*other.GetRootItem()));
}

SessionModel::~SessionModel()
{
  // Explicitely call root item's destructor. It uses p_impl pointer during own descruction
  // and we have to keep pimpl pointer intact. Without line below will crash on MacOS because
  // of pecularities of MacOS libc++. See explanations here:
  // http://ibob.github.io/blog/2019/11/07/dont-use-unique_ptr-for-pimpl/
  p_impl->m_root_item.reset();
}

std::unique_ptr<ISessionModel> SessionModel::Clone() const
{
  return std::make_unique<SessionModel>(*this);
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

ICommandStack* SessionModel::GetCommandStack() const
{
  return p_impl->m_command_processor.get();
}

SessionItem* SessionModel::InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                      const TagIndex& tag_index)
{
  if (!parent)
  {
    parent = GetRootItem();
  }

  return parent->InsertItem(std::move(item), tag_index);
}

std::unique_ptr<SessionItem> SessionModel::TakeItem(SessionItem* parent, const TagIndex& tag_index)
{
  utils::ValidateTakeItem(parent, tag_index);
  return parent->TakeItem(tag_index);
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

  // moving to the same position we are now is noop (no move, no notifications)
  if (item->GetParent() == new_parent && item->GetTagIndex() == actual_tagindex)
  {
    return;
  }

  utils::ValidateItemMove(item, new_parent, actual_tagindex);
  utils::BeginMacro(*this, "MoveItem");
  auto taken = TakeItem(item->GetParent(), item->GetTagIndex());
  InsertItem(std::move(taken), new_parent, tag_index);
  utils::EndMacro(*this);
}

bool SessionModel::SetData(SessionItem* item, const variant_t& value, role_t role)
{
  return item->SetDataInternal(value, role);
}

SessionItem* SessionModel::FindItem(const std::string& id) const
{
  return p_impl->m_pool->ItemForKey(id);
}

void SessionModel::Clear()
{
  ReplaceRootItem(utils::CreateEmptyRootItem());
}

void SessionModel::ReplaceRootItem(std::unique_ptr<SessionItem> root_item)
{
  if (!root_item)
  {
    throw RuntimeException("Uninitialised root item");
  }

  p_impl->m_root_item = std::move(root_item);
  p_impl->m_root_item->SetModel(this);
  p_impl->m_command_processor->Clear();
}

void SessionModel::CheckIn(SessionItem* item)
{
  p_impl->m_pool->RegisterItem(item, item->GetIdentifier());
}

void SessionModel::CheckOut(SessionItem* item)
{
  p_impl->m_pool->UnregisterItem(item);
}

ICommand* SessionModel::ExecuteCommand(std::unique_ptr<ICommand> command)
{
  if (p_impl->m_is_executing)
  {
    throw RuntimeException("Command still executing");
  }

  p_impl->m_is_executing = true;
  ICommand* result{nullptr};
  try
  {
    result = p_impl->m_command_processor->Execute(std::move(command));
    p_impl->m_is_executing = false;
  }
  catch (const std::exception& ex)
  {
    std::ostringstream ostr;
    ostr << "Exception was caught while trying to execute command ";
    ostr << "with the message [" << ex.what() << "]";
    p_impl->m_is_executing = false;
    throw InvalidOperationException(ostr.str());
  }
  return result;
}

void SessionModel::SetCommandProcessor(std::unique_ptr<ICommandStack> command_processor)
{
  p_impl->m_command_processor = std::move(command_processor);
}

}  // namespace mvvm
