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

#ifndef MVVM_COMMANDS_COMMAND_MODEL_COMPOSER_H_
#define MVVM_COMMANDS_COMMAND_MODEL_COMPOSER_H_

#include <mvvm/commands/command_stack_interface.h>
#include <mvvm/interfaces/model_composer_interface.h>

#include <memory>

namespace mvvm
{

class CommandStackInterface;

/**
 * @brief The CommandModelComposer class is a decorator class to enhance standard ModelComposer with
 * undo/redo capabilities.
 */
class CommandModelComposer : public ModelComposerInterface
{
public:
  explicit CommandModelComposer(CommandStackInterface* command_stack,
                                std::unique_ptr<ModelComposerInterface> composer)
      : m_command_stack(command_stack), m_composer(std::move(composer))
  {
  }

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

  void Reset(std::unique_ptr<SessionItem>& old_root_item,
             std::unique_ptr<SessionItem> new_root_item) override;

  ISessionModel* GetModel() const override;

private:
  template <typename C, typename... Args>
  C* ProcessCommand(Args&&... args);

  CommandStackInterface* m_command_stack{nullptr};
  std::unique_ptr<ModelComposerInterface> m_composer;
};

template <typename C, typename... Args>
C* CommandModelComposer::ProcessCommand(Args&&... args)
{
  auto command = std::make_unique<C>(std::forward<Args>(args)...);
  return static_cast<C*>(m_command_stack->Execute(std::move(command)));
}

}  // namespace mvvm

#endif  // MVVM_COMMANDS_COMMAND_MODEL_COMPOSER_H_
