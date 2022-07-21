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

//! Base composer to build the model.

class ModelComposer : public ModelComposerInterface
{
  explicit ModelComposer(SessionModelInterface& model);

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  void RemoveItem(SessionItem* item) override;

  void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index) override;

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

private:
  SessionModelInterface& m_model;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_ITEM_UTILS_H_
