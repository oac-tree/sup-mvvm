/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_SESSION_MODEL_H_
#define MVVM_MODEL_SESSION_MODEL_H_

#include <mvvm/model/i_session_model.h>

#include <memory>

namespace mvvm
{

class SessionItem;
class ItemPool;
class IModelComposer;

/**
 * @brief The SessionModel class is the main model to hold hierarchy of SessionItem objects.
 */
class MVVM_MODEL_EXPORT SessionModel : public ISessionModel
{
public:
  using ISessionModel::InsertItem;

  explicit SessionModel(std::string model_type = {});

  SessionModel(std::string model_type, std::shared_ptr<ItemPool> pool,
               std::unique_ptr<IModelComposer> composer = {});

  ~SessionModel() override;
  SessionModel(const SessionModel& other) = delete;
  SessionModel& operator=(const SessionModel& other) = delete;

  std::string GetType() const override;

  SessionItem* GetRootItem() const override;

  ModelEventHandler* GetEventHandler() const override;

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  void RemoveItem(SessionItem* item) override;

  void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index) override;

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

  SessionItem* FindItem(const std::string& id) const override;

  void Clear() override;

  void ReplaceRootItem(std::unique_ptr<SessionItem> root_item) override;

  void CheckIn(SessionItem* item) override;

  void CheckOut(SessionItem* item) override;

  /**
   * @brief Sets internal composer to given value.
   */
  void SetComposer(std::unique_ptr<IModelComposer> composer);

private:
  friend class SessionItem;

  struct SessionModelImpl;
  std::unique_ptr<SessionModelImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSIONMODEL_H_
