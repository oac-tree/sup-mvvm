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

#ifndef MVVM_COMMANDS_MODEL_COMMAND_H_
#define MVVM_COMMANDS_MODEL_COMMAND_H_

#include <mvvm/commands/abstract_command.h>

namespace mvvm
{

class ISessionModel;

/**
 * @brief The ModelCommand is a base for all commands providing notifications.
 */
class MVVM_MODEL_EXPORT ModelCommand : public AbstractCommand
{
public:
  explicit ModelCommand(ISessionModel* model);
  ~ModelCommand() override = default;

  ModelCommand(const ModelCommand&) = delete;
  ModelCommand& operator=(const ModelCommand&) = delete;
  ModelCommand(ModelCommand&&) noexcept = delete;
  ModelCommand& operator=(ModelCommand&&) noexcept = delete;

  ISessionModel* GetModel() const;

  SessionItem* FindItem(const std::string& identifier) const;

private:
  void Notify(const event_variant_t& event) override;

  ISessionModel* m_model{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_COMMANDS_MODEL_COMMAND_H_
