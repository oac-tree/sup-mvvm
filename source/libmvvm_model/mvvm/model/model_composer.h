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

#ifndef MVVM_MODEL_MODEL_COMPOSER_H_
#define MVVM_MODEL_MODEL_COMPOSER_H_

#include <mvvm/interfaces/model_composer_interface.h>

namespace mvvm
{

class SessionModelInterface;

//! The standard ModelComposer to modify the model (add/remove items, reset the model).
//! Doesn't contains any undo/redo, or signaling and is used for decorations.

class ModelComposer : public ModelComposerInterface
{
public:
  explicit ModelComposer(SessionModelInterface& model);

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

  void Reset(std::unique_ptr<SessionItem>& old_root_item,
             std::unique_ptr<SessionItem> new_root_item) override;

  SessionModelInterface* GetModel() const override;

private:
  SessionModelInterface& m_model;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_MODEL_COMPOSER_H_
