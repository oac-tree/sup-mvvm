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

#ifndef MVVM_INTERFACES_MODEL_COMPOSER_INTERFACE_H_
#define MVVM_INTERFACES_MODEL_COMPOSER_INTERFACE_H_

#include <mvvm/core/variant.h>
#include <mvvm/model_export.h>

#include <functional>
#include <memory>

namespace mvvm
{
class SessionItem;
class TagIndex;

//! Interface for all classes to compose the model.
//! Used in various decorators for undo/redo and notifications while composing the model.

class MVVM_MODEL_EXPORT ModelComposerInterface
{
public:
  virtual ~ModelComposerInterface() = default;

  virtual SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                  const TagIndex& tag_index) = 0;

  virtual std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) = 0;

  virtual void RemoveItem(SessionItem* item) = 0;

  virtual void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index) = 0;

  virtual bool SetData(SessionItem* item, const variant_t& value, int role) = 0;
};

}  // namespace mvvm

#endif  // MVVM_INTERFACES_MODEL_COMPOSER_INTERFACE_H_
