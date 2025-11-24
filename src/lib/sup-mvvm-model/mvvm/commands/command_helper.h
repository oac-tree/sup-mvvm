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
#include <mvvm/core/variant.h>
#include <mvvm/signals/event_types.h>

namespace mvvm
{

class SessionItem;
class TagIndex;

/**
 * @brief Returns event which has to be send before InsertCommand execution or undo.
 *
 * @param status The current status of the command.
 * @param parent The parent to insert.
 * @param tag_index Address of a future child.
 *
 * @return An event to send.
 */
event_variant_t GetBeforeEventForInsertCommand(CommandStatus status, SessionItem* parent,
                                               const TagIndex& tag_index);

/**
 * @brief Returns event which has to be send after InsertCommand execution or undo.
 *
 * @param status The current status of the command.
 * @param parent The parent to insert.
 * @param tag_index Address of a future child.
 *
 * @return An event to send.
 */
event_variant_t GetAfterEventForInsertCommand(CommandStatus status, SessionItem* parent,
                                              const TagIndex& tag_index);

/**
 * @brief Returns event which has to be send before RemoveCommand execution or undo.
 *
 * @param status The current status of the command.
 * @param parent The parent to insert.
 * @param tag_index Address of a future child.
 *
 * @return An event to send.
 */
event_variant_t GetBeforeEventForRemoveCommand(CommandStatus status, SessionItem* parent,
                                               const TagIndex& tag_index);

/**
 * @brief Returns event which has to be send after RemoveCommand execution or undo.
 *
 * @param status The current status of the command.
 * @param parent The parent to insert.
 * @param tag_index Address of a future child.
 *
 * @return An event to send.
 */
event_variant_t GetAfterEventForRemoveCommand(CommandStatus status, SessionItem* parent,
                                              const TagIndex& tag_index);

/**
 * @brief Returns text description for SetValue command.
 */
std::string GetSetValueCommandDescription(const variant_t& value, role_t role);

/**
 * @brief Returns text description for insert command.
 */
std::string GetInsertCommandDescription(const SessionItem *parent, const SessionItem *child,
                                        const mvvm::TagIndex& tag_index);

/**
 * @brief Returns text description for insert command.
 */
std::string GetRemoveCommandDescription(mvvm::SessionItem* parent, const mvvm::TagIndex& tag_index);

}  // namespace mvvm

#endif  // MVVM_COMMANDS_COMMAND_HELPER_H_
