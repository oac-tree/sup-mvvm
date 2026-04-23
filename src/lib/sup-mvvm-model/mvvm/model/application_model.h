/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_MODEL_APPLICATION_MODEL_H_
#define MVVM_MODEL_APPLICATION_MODEL_H_

#include <mvvm/model/session_model.h>

#include <memory>

namespace mvvm
{

class ModelEventHandler;

/**
 * @brief The ApplicationModel class is the main model to hold application data.
 *
 * It extends SessionModel with signalling capabilites, and undo/redo.
 */
class ApplicationModel : public SessionModel
{
public:
  explicit ApplicationModel(std::string model_type = {});
  ApplicationModel(std::string model_type, std::shared_ptr<ItemPool> pool);

  ~ApplicationModel() override;

  ApplicationModel(const ApplicationModel& other);
  ApplicationModel& operator=(const ApplicationModel&) = delete;
  ApplicationModel(ApplicationModel&&) = delete;
  ApplicationModel& operator=(ApplicationModel&&) = delete;

  std::unique_ptr<ISessionModel> Clone() const override;

  ModelEventHandler* GetEventHandler() const override;

  void ReplaceRootItem(std::unique_ptr<SessionItem> root_item) override;

  /**
   * @brief Enables or disables undo/redo framework.
   *
   * When disabled, all command history is cleared.
   *
   * @param value To enable undo.
   * @param undo_limit Maximum number of commands stored in undo stack (0 = no limits).
   */
  void SetUndoEnabled(bool value, std::size_t undo_limit = 0);

private:
  std::unique_ptr<ModelEventHandler> m_event_handler;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_APPLICATION_MODEL_H_
