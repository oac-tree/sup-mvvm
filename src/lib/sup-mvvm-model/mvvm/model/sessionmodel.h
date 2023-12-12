/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_SESSIONMODEL_H_
#define MVVM_MODEL_SESSIONMODEL_H_

#include <mvvm/interfaces/model_composer_interface.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/function_types.h>
#include <mvvm/model/sessionitem.h>

#include <memory>

namespace mvvm
{
class SessionItem;
class ItemManagerInterface;
class ItemFactoryInterface;

//! Main class to hold hierarchy of SessionItem objects.

class MVVM_MODEL_EXPORT SessionModel : public SessionModelInterface
{
public:
  using SessionModelInterface::InsertItem;

  explicit SessionModel(std::string model_type = {});

  SessionModel(std::string model_type, std::unique_ptr<ItemManagerInterface> manager,
               std::unique_ptr<ModelComposerInterface> composer = {});

  ~SessionModel() override;
  SessionModel(const SessionModel& other) = delete;
  SessionModel& operator=(const SessionModel& other) = delete;

  std::string GetType() const override;

  SessionItem* GetRootItem() const override;

  ModelEventHandler* GetEventHandler() const override;

  // Methods to manipulate data and items.

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  void RemoveItem(SessionItem* item) override;

  void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index) override;

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

  // Various getters.

  SessionItem* FindItem(const std::string& id) const override;

  // Methods to steer global behaviour.

  void Clear(std::unique_ptr<SessionItem> root_item) override;

  void CheckIn(SessionItem* item) override;

  void CheckOut(SessionItem* item) override;

  void SetComposer(std::unique_ptr<ModelComposerInterface> composer);

private:
  friend class SessionItem;

  struct SessionModelImpl;
  std::unique_ptr<SessionModelImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSIONMODEL_H_
