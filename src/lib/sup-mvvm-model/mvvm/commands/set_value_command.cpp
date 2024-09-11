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

#include "set_value_command.h"

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

struct SetValueCommand::SetValueCommandImpl
{
  IModelComposer *m_composer{nullptr};
  variant_t m_value;  //! Value to set as a result of command execution.
  int m_role;
  Path m_item_path;
  bool m_result{false};

  SetValueCommandImpl(IModelComposer *composer, const variant_t &value, int role)
      : m_composer(composer), m_value(value), m_role(role)
  {
  }
};

SetValueCommand::SetValueCommand(IModelComposer *composer, SessionItem *item,
                                 const variant_t &value, int role)
    : p_impl(std::make_unique<SetValueCommandImpl>(composer, value, role))
{
  SetDescription(GenerateDescription(p_impl->m_value, role));
  // saving persistent path to item to be able to find it even after destruction
  p_impl->m_item_path = utils::PathFromItem(item);
}

bool SetValueCommand::GetResult() const
{
  return p_impl->m_result;
}

SetValueCommand::~SetValueCommand() = default;

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
  auto item = utils::ItemFromPath(*p_impl->m_composer->GetModel(), p_impl->m_item_path);
  auto old = item->Data(p_impl->m_role);
  auto result = p_impl->m_composer->SetData(item, p_impl->m_value, p_impl->m_role);
  SetResult(result);
  SetIsObsolete(!result);
  p_impl->m_value = old;
}

void SetValueCommand::SetResult(bool value)
{
  p_impl->m_result = value;
}

}  // namespace mvvm
