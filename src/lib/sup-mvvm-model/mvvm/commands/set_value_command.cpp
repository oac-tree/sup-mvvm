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
#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>

namespace mvvm
{

SetValueCommand::SetValueCommand(SessionItem *item, const variant_t &value, role_t role)
    : ModelCommand(item ? item->GetModel() : nullptr)
    , m_cached_item(item)
    , m_value(value)
    , m_role(role)
{
  if (!m_cached_item)
  {
    throw RuntimeException("Uninitialized item");
  }

  m_item_identifier = m_cached_item->GetIdentifier();
}

SetValueCommand::~SetValueCommand() = default;

SetValueCommand::result_t SetValueCommand::GetResult() const
{
  return m_result;
}

std::optional<event_variant_t> SetValueCommand::GetEventBefore() const
{
  m_cached_item = FindItemToSet();
  return {};
}

std::optional<event_variant_t> SetValueCommand::GetEventAfter() const
{
  if (m_result)
  {
    return DataChangedEvent{m_cached_item, m_role};
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
  auto old = m_cached_item->Data(m_role);
  auto result = utils::SetDataImpl(*m_cached_item, m_value, m_role);
  SetResult(result);
  SetIsObsolete(!result);
  m_value = old;
}

void SetValueCommand::SetResult(bool value)
{
  m_result = value;
}

SessionItem *SetValueCommand::FindItemToSet() const
{
  auto found_item = FindItem(m_item_identifier);
  return found_item ? found_item : m_cached_item;
}

}  // namespace mvvm
