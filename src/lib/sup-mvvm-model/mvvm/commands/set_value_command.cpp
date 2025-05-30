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

#include "set_value_command.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/i_session_model.h>
#include <mvvm/model/session_item.h>

namespace mvvm
{

struct SetValueCommand::SetValueCommandImpl
{
  SessionItem *m_cached_item{nullptr};
  ISessionModel *m_model{nullptr};
  variant_t m_value;  //! Value to set as a result of command execution.
  std::int32_t m_role;
  std::string m_item_identifier;
  bool m_result{false};

  SetValueCommandImpl(SessionItem *item, const variant_t &value, int role)
      : m_cached_item(item), m_value(value), m_role(role)
  {
    if (!m_cached_item)
    {
      throw RuntimeException("Uninitialized item");
    }

    m_model = m_cached_item->GetModel();
    m_item_identifier = m_cached_item->GetIdentifier();
  }

  SessionItem *FindItem() const
  {
    return m_model ? m_model->FindItem(m_item_identifier) : m_cached_item;
  }

  void SetResult(bool value) { m_result = value; }
};

SetValueCommand::SetValueCommand(SessionItem *item, const variant_t &value, int role)
    : p_impl(std::make_unique<SetValueCommandImpl>(item, value, role))
{
}

SetValueCommand::~SetValueCommand() = default;

bool SetValueCommand::GetResult() const
{
  return p_impl->m_result;
}

std::optional<event_variant_t> SetValueCommand::GetEventBefore() const
{
  p_impl->m_cached_item = p_impl->FindItem();
  return {};
}

std::optional<event_variant_t> SetValueCommand::GetEventAfter() const
{
  if (p_impl->m_result)
  {
    return DataChangedEvent{p_impl->m_cached_item, p_impl->m_role};
  }

  return {};
}

void SetValueCommand::ExecuteImpl()
{
  SwapValues();
}

void SetValueCommand::UndoImpl()
{
  SwapValues();
}

void SetValueCommand::SwapValues()
{
  auto old = p_impl->m_cached_item->Data(p_impl->m_role);
  auto result = p_impl->m_cached_item->SetDataImpl(p_impl->m_value, p_impl->m_role);
  SetResult(result);
  SetIsObsolete(!result);
  p_impl->m_value = old;
}

void SetValueCommand::SetResult(bool value)
{
  p_impl->SetResult(value);
}

}  // namespace mvvm
