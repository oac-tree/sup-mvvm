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
  Path m_item_path;
  bool m_result{false};

  SetValueCommandImpl(SessionItem *item, const variant_t &value, int role)
      : m_item(item), m_model(m_item->GetModel()), m_value(value), m_role(role)
  {
  }
};

SetValueCommandV2::SetValueCommandV2(SessionItem *item, const variant_t &value, int role)
    : p_impl(std::make_unique<SetValueCommandImpl>(item, value, role))
{
  SetDescription(GenerateDescription(p_impl->m_value, role));
  p_impl->m_item_path = utils::PathFromItem(item);
}

bool SetValueCommandV2::GetResult() const
{
  return p_impl->m_result;
}

SetValueCommandV2::~SetValueCommandV2() = default;

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
}

void SetValueCommandV2::SetResult(bool value)
{
  p_impl->m_result = value;
}

}  // namespace mvvm
