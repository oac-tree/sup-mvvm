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

#ifndef MVVM_MODEL_SESSION_MODEL_V2_H_
#define MVVM_MODEL_SESSION_MODEL_V2_H_

#include <mvvm/model/session_model.h>

namespace mvvm
{

class ICommand;

class SessionModelV2 : public SessionModel
{
public:
  SessionModelV2();

  ICommand* ExecuteCommand(std::unique_ptr<ICommand> command);

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

private:
  std::unique_ptr<ICommandStack> m_command_stack;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSION_MODEL_V2_H_
