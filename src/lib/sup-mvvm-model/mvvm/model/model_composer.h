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

#ifndef MVVM_MODEL_MODEL_COMPOSER_H_
#define MVVM_MODEL_MODEL_COMPOSER_H_

#include <mvvm/model/i_model_composer.h>

namespace mvvm
{

/**
 * @brief The ModelComposer class represents the basic composer to modify the model.
 *
 * It doesn't provide any notifications, or undo/redo, and simply forwards the method call to the
 * corresponding model's API.
 */
class ModelComposer : public IModelComposer
{
public:
  explicit ModelComposer(ISessionModel& model);

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

  void ReplaceRootItem(std::unique_ptr<SessionItem>& old_root_item,
                       std::unique_ptr<SessionItem> new_root_item) override;

  ISessionModel* GetModel() const override;

private:
  ISessionModel& m_model;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_MODEL_COMPOSER_H_
