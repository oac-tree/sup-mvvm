/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_INTERFACES_SESSIONMODELINTERFACE_H
#define MVVM_INTERFACES_SESSIONMODELINTERFACE_H

#include "mvvm/core/variant.h"
#include "mvvm/model/tagindex.h"
#include "mvvm/model_export.h"

#include <functional>

namespace ModelView
{
class SessionItem;

//! Application model interface.

class MVVM_MODEL_EXPORT SessionModelInterface
{
public:
  virtual ~SessionModelInterface() = default;

  virtual SessionItem* InsertNewItem(const std::string& item_type, SessionItem* parent,
                                     const TagIndex& tag_index) = 0;

  template <typename T>
  T* InsertItem(SessionItem* parent = nullptr, const TagIndex& tag_index = {});

  virtual void RemoveItem(SessionItem* parent, const TagIndex& tag_index) = 0;

  virtual bool SetData(SessionItem* item, const variant_t& value, int role) = 0;

  virtual void Clear(std::function<void(SessionItem*)> callback = {}) = 0;
};

//! Inserts item into given parent under given tag_index.

template <typename T>
T* SessionModelInterface::InsertItem(SessionItem* parent, const TagIndex& tag_index)
{
  return static_cast<T*>(InsertNewItem(T().GetType(), parent, tag_index));
}

}  // namespace ModelView

#endif  // MVVM_INTERFACES_SESSIONMODELINTERFACE_H
