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

#include "set_value_command_v2.h"

#include <mvvm/model/i_model_composer.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/path.h>
#include <mvvm/model/session_item.h>

#include <sstream>

namespace
{

std::string GenerateDescription(const mvvm::variant_t &value, int role)
{
  (void)value;
  std::ostringstream ostr;
  ostr << "Set value: " << mvvm::utils::ValueToString(value) << ", role:" << role;
  return ostr.str();
}

}  // namespace

namespace mvvm
{

struct SetValueCommandV2::SetValueCommandImpl
{
  SessionItem *m_item{nullptr};
  ISessionModel *m_model{nullptr};
  variant_t m_value;  //! Value to set as a result of command execution.
  std::int32_t m_role;
  std::string m_item_identifier;
  bool m_result{false};
  std::optional<event_variant_t> m_next_event;

  SetValueCommandImpl(SessionItem *item, const variant_t &value, int role)
      : m_item(item), m_model(m_item->GetModel()), m_value(value), m_role(role)
  {
    m_item_identifier = item->GetIdentifier();
  }

  SessionItem *FindItem() const { return m_model ? m_model->FindItem(m_item_identifier) : m_item; }

  void SetResult(bool value)
  {
    m_result = value;
    m_next_event =
        m_result ? std::optional<event_variant_t>(DataChangedEvent{m_item, m_role}) : std::nullopt;
  }
};

SetValueCommandV2::SetValueCommandV2(SessionItem *item, const variant_t &value, int role)
    : p_impl(std::make_unique<SetValueCommandImpl>(item, value, role))
{
  SetDescription(GenerateDescription(p_impl->m_value, role));
}

SetValueCommandV2::~SetValueCommandV2() = default;

bool SetValueCommandV2::GetResult() const
{
  return p_impl->m_result;
}

std::optional<event_variant_t> SetValueCommandV2::GetEventBefore() const
{
  return {};
}

std::optional<event_variant_t> SetValueCommandV2::GetEventAfter() const
{
  return p_impl->m_next_event;
}

void SetValueCommandV2::ExecuteImpl()
{
  SwapValues();
}

void SetValueCommandV2::UndoImpl()
{
  SwapValues();
}

void SetValueCommandV2::SwapValues()
{
  p_impl->m_item = p_impl->FindItem();
  auto old = p_impl->m_item->Data(p_impl->m_role);
  auto result = p_impl->m_item->SetDataImpl(p_impl->m_value, p_impl->m_role);
  SetResult(result);
  SetIsObsolete(!result);
  p_impl->m_value = old;
}

void SetValueCommandV2::SetResult(bool value)
{
  p_impl->SetResult(value);
}

}  // namespace mvvm
