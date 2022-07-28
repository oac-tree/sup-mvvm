/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_COMMANDS_COMMAND_MODEL_COMPOSER_H_
#define MVVM_COMMANDS_COMMAND_MODEL_COMPOSER_H_

#include <mvvm/interfaces/model_composer_interface.h>
#include <mvvm/commands/command_stack_interface.h>
#include <mvvm/model/sessionitem.h>

namespace mvvm
{

class CommandStackInterface;

//! The decorator class to enhance standard ModelComposer with notification abilities.

template <typename T>
class CommandModelComposer : public T
{
public:
  static_assert(std::is_base_of<ModelComposerInterface, T>::value, "Invalid template argument");

  template <typename... Args>
  explicit CommandModelComposer(CommandStackInterface* command_stack, Args&&... args)
      : m_command_stack(command_stack), T(std::forward<Args>(args)...)
  {
  }

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override
  {
    return {};
  }

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override
  {
    return {};
  }

  bool SetData(SessionItem* item, const variant_t& value, int role) override
  {
    return {};
  }

  void Reset(std::unique_ptr<SessionItem>& old_root_item,
             std::unique_ptr<SessionItem> new_root_item) override
  {
  }

private:
  CommandStackInterface* m_command_stack{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_COMMAND_MODEL_COMPOSER_H_
