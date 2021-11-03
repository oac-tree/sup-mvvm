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
#include "mvvm/interfaces/sessionmodelinterface.h"
#include "mvvm/model/tagindex.h"
#include "mvvm/model_export.h"

#include <functional>
#include <memory>

namespace ModelView
{
class SessionModel;
class SessionItem;
class ModelEventListenerInterface;
class ModelEventNotifierInterface;

//! The ModelComposer class allows manipulate the model (add/remove items, set item's data) and
//! generate necessary notifications.
//!
//! Notification are generated with the help of ModelEventNotifier. Any ModelEventListener
//! can connect and listen.
//!
//! TODO Consider switching to the decorator design pattern (SessionModelDecorator).
//! TODO Consider merging ApplicationModel and ModelComposer

class MVVM_MODEL_EXPORT ModelComposer : public SessionModelInterface
{
public:
  using SessionModelInterface::InsertItem;

  ModelComposer(SessionModel* model, ModelEventNotifierInterface* notifier);
  virtual ~ModelComposer();

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  SessionItem* InsertNewItem(const std::string& item_type, SessionItem* parent = nullptr,
                             const TagIndex& tag_index = {}) override;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  void RemoveItem(SessionItem* item) override;

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

  void Clear(std::function<void(SessionItem*)> callback) override;

private:
  struct ModelComposerImpl;
  std::unique_ptr<ModelComposerImpl> p_impl;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_MODELCOMPOSER_H
