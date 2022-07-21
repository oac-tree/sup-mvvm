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

#ifndef MVVM_MODEL_NOTIFYING_MODEL_H_
#define MVVM_MODEL_NOTIFYING_MODEL_H_

#include <mvvm/interfaces/sessionmodel_interface.h>

namespace mvvm
{
class ModelEventSubscriberInterface;
class ItemManagerInterface;

//! A decorator to provide SessionModel with signaling capabilities.

class MVVM_MODEL_EXPORT NotifyingModel : public SessionModelInterface
{
public:
  using SessionModelInterface::InsertItem;

  explicit NotifyingModel(std::unique_ptr<SessionModelInterface> decorated_model);

  ~NotifyingModel() override;

  std::string GetType() const override;

  SessionItem* GetRootItem() const override;

  const ItemFactoryInterface* GetFactory() const override;

  ModelEventSubscriberInterface* GetSubscriber() const override;

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  void RemoveItem(SessionItem* item) override;

  void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index) override;

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

  SessionItem* FindItem(const std::string& id) const override;

  void Clear(std::unique_ptr<SessionItem> root_item, SessionModelInterface*model) override;

  void CheckIn(SessionItem* item) override;

  void CheckOut(SessionItem* item) override;

private:
  const SessionModelInterface* GetDecoratedModel() const;
  SessionModelInterface* GetDecoratedModel();

  struct NotifyingModelImpl;
  std::unique_ptr<NotifyingModelImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_NOTIFYING_MODEL_H_
