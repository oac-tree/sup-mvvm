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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_APPLICATION_MODEL_COMPOSER_H_
#define MVVM_MODEL_APPLICATION_MODEL_COMPOSER_H_

#include <mvvm/model/i_model_composer.h>

#include <memory>

namespace mvvm
{

class ModelEventHandler;
class ICommandStack;

class ApplicationModelComposer : public IModelComposer
{
public:
  ApplicationModelComposer(ISessionModel* model, ModelEventHandler* event_handler,
                           ICommandStack* command_stack);
  ~ApplicationModelComposer() override;

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

  void ReplaceRootItem(std::unique_ptr<SessionItem>& old_root_item,
                       std::unique_ptr<SessionItem> new_root_item) override;

  ISessionModel* GetModel() const override;

private:
  std::unique_ptr<IModelComposer> m_model_composer;
  ModelEventHandler* m_event_handler{nullptr};
  ICommandStack* m_command_stack{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_MODEL_APPLICATION_MODEL_COMPOSER_H_
