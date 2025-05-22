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

#ifndef MVVM_COMMANDS_COMMAND_HELPER_H_
#define MVVM_COMMANDS_COMMAND_HELPER_H_

#include <mvvm/commands/command_status.h>
#include <mvvm/signals/event_types.h>

namespace mvvm
{

class SessionItem;
class TagIndex;

event_variant_t GetBeforeEventForInsertCommand(CommandStatus status, SessionItem* parent,
                                            const TagIndex& tag_index);

event_variant_t GetAfterEventForInsertCommand(CommandStatus status, SessionItem* parent,
                                            const TagIndex& tag_index);

}  // namespace mvvm

#endif  // MVVM_COMMANDS_COMMAND_HELPER_H_
