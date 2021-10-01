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

#ifndef MVVM_MODEL_MODELCOMPOSER_H
#define MVVM_MODEL_MODELCOMPOSER_H

#include "mvvm/core/variant.h"
#include "mvvm/model/tagindex.h"
#include "mvvm/model_export.h"

#include <memory>

namespace ModelView
{
class SessionModel;
class SessionItem;
class ModelEventListenerInterface;
class ModelEventNotifierInterface;

//! Interface class for model modifications.
//!
//! It is used to modify the model (add/remove items, set item's data) via usual
//! model API, and perform additional activities.
//!
//! The ModelComposer class allows manipulate the model and generate necessary notifications.
//! TODO Consider switching to the decorator design pattern (SessionModelDecorator).

class MVVM_MODEL_EXPORT ModelComposer
{
public:
  ModelComposer(SessionModel* model, ModelEventNotifierInterface* notifier);
  virtual ~ModelComposer();

  void EstablishConnections(ModelEventListenerInterface* listener);

  bool SetData(SessionItem* item, const variant_t& value, int role);

  SessionItem* InsertNewItem(const std::string& item_type, SessionItem* parent = nullptr,
                             const TagIndex& tag_index = {});

  template <typename T>
  T* InsertItem(SessionItem* parent = nullptr, const TagIndex& tag_index = {});

  void RemoveItem(SessionItem* parent, const TagIndex& tag_index);

private:
  struct ModelComposerImpl;
  std::unique_ptr<ModelComposerImpl> p_impl;
};

template <typename T>
T* ModelComposer::InsertItem(SessionItem* parent, const TagIndex& tag_index)
{
  return static_cast<T*>(InsertNewItem(T::Type, parent, tag_index));
}

}  // namespace ModelView

#endif  // MVVM_MODEL_MODELCOMPOSER_H
