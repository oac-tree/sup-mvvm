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
  ICommand* GetNextUndoCommand();
  ICommand* GetNextRedoCommand();
  void NotifyBefore(ICommand* command);
  void NotifyAfter(ICommand* command);

  notify_callback_t m_notify_callback;
};

template <typename T>
inline ICommand* NotifyingCommandStack<T>::Execute(std::unique_ptr<ICommand> command)
{
  auto command_ptr = command.get();
  NotifyBefore(command_ptr);
  T::Execute(std::move(command));
  NotifyAfter(command_ptr);
  return command_ptr;
}

template <typename T>
inline void NotifyingCommandStack<T>::Undo()
{
  auto command = GetNextUndoCommand();

  NotifyBefore(command);

  T::Undo();

  NotifyAfter(command);
}

template <typename T>
inline void NotifyingCommandStack<T>::Redo()
{
  auto command = GetNextRedoCommand();

  NotifyBefore(command);

  T::Redo();

  NotifyAfter(command);
}

template <typename T>
inline ICommand* NotifyingCommandStack<T>::GetNextUndoCommand()
{
  return nullptr;
}

template <typename T>
inline ICommand* NotifyingCommandStack<T>::GetNextRedoCommand()
{
  return nullptr;
}

template <typename T>
inline void NotifyingCommandStack<T>::NotifyBefore(ICommand* command)
{
  (void)command;
}

template <typename T>
inline void NotifyingCommandStack<T>::NotifyAfter(ICommand* command)
{
  (void)command;
}

}  // namespace mvvm

#endif  // MVVM_COMMANDS_NOTIFYING_COMMAND_STACK_H_
