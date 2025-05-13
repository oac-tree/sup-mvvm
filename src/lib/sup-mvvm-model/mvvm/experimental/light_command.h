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

#ifndef MVVM_EXPERIMENTAL_LIGHT_COMMAND_H_
#define MVVM_EXPERIMENTAL_LIGHT_COMMAND_H_

#include <mvvm/commands/abstract_command.h>
#include <mvvm/signals/event_types.h>

#include <optional>

namespace mvvm::experimental
{

class LightCommand : public AbstractCommand
{
public:
  virtual std::optional<event_variant_t> GetNextEvent() const = 0;
};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_LIGHT_COMMAND_H_
