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

#ifndef MVVM_MODEL_APPLICATION_MODEL_H_
#define MVVM_MODEL_APPLICATION_MODEL_H_

#include <mvvm/model/abstract_model_decorator.h>

namespace mvvm
{
class ModelEventSubscriberInterface;
class ItemManagerInterface;

//! Main class to hold application data. Extends SessionModel with signaling capabilities.
//! All modification of the model are done through the composer, which takes care
//! about notifications and all subscribers.

class MVVM_MODEL_EXPORT ApplicationModel : public AbstractModelDecorator
{
public:
  using SessionModelInterface::InsertItem;

  explicit ApplicationModel(std::string model_type = {});
  ApplicationModel(std::string model_type, std::unique_ptr<ItemManagerInterface> manager);

  ~ApplicationModel() override;

  ModelEventSubscriberInterface* GetSubscriber() const override;

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  void RemoveItem(SessionItem* item) override;

  void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index) override;

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

  void Clear(std::unique_ptr<SessionItem> root_item, SessionModelInterface* model) override;

  void CheckIn(SessionItem* item) override;

private:
  struct ApplicationModelImpl;
  std::unique_ptr<ApplicationModelImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_APPLICATION_MODEL_H_
