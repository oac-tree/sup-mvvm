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

#ifndef MVVM_COMMANDS_NOTIFYING_COMMAND_STACK_H_
#define MVVM_COMMANDS_NOTIFYING_COMMAND_STACK_H_

#include <mvvm/commands/i_command.h>
#include <mvvm/commands/i_command_stack.h>
#include <mvvm/signals/event_types.h>

#include <functional>

namespace mvvm
{

/**
 * @brief The NotifyingCommandStack class extends command stack with notification capabilities.
 */
template <typename T>
class MVVM_MODEL_EXPORT NotifyingCommandStack : public T
{
public:
  using notify_callback_t = typename std::function<void(const event_variant_t&)>;

  template <typename... Args>
  explicit NotifyingCommandStack(const notify_callback_t& notify_callback, Args&&... args)
      : T(std::forward<Args>(args)...), m_notify_callback(notify_callback)
  {
  }

  ICommand* Execute(std::unique_ptr<ICommand> command) override;

  void Undo() override;

  void Redo() override;

private:
  void Notify(const std::optional<event_variant_t>& event);

  notify_callback_t m_notify_callback;
};

template <typename T>
inline ICommand* NotifyingCommandStack<T>::Execute(std::unique_ptr<ICommand> command)
{
  auto command_ptr = command.get();
  Notify(command_ptr->GetEventBefore());
  // check result so command is not obsolete and still alive after execution
  if (auto result = T::Execute(std::move(command)); result)
  {
    Notify(result->GetEventAfter());
  }
  return command_ptr;
}

template <typename T>
inline void NotifyingCommandStack<T>::Undo()
{
  if (const auto command = T::GetNextUndoCommand(); command)
  {
    Notify(command->GetEventBefore());
    T::Undo();
    Notify(command->GetEventAfter());
  }
}

template <typename T>
inline void NotifyingCommandStack<T>::Redo()
{
  if (const auto command = T::GetNextRedoCommand(); command)
  {
    Notify(command->GetEventBefore());
    T::Redo();
    Notify(command->GetEventAfter());
  }
}

template <typename T>
inline void NotifyingCommandStack<T>::Notify(const std::optional<event_variant_t>& event)
{
  if (event.has_value())
  {
    m_notify_callback(event.value());
  }
}

}  // namespace mvvm

#endif  // MVVM_COMMANDS_NOTIFYING_COMMAND_STACK_H_
