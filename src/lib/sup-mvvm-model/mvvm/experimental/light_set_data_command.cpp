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

#include "light_set_data_command.h"

namespace mvvm::experimental
{

LightSetDataCommand::LightSetDataCommand(LightItem *item, const variant_t &value, int32_t role)
{
  (void)item;
  (void)value;
  (void)role;
}

std::optional<event_variant_t> LightSetDataCommand::GetNextEvent() const
{
  return {};
}

void LightSetDataCommand::ExecuteImpl() {}

void LightSetDataCommand::UndoImpl() {}

}  // namespace mvvm::experimental
