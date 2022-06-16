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

#ifndef MVVM_MODEL_SESSIONMODEL_H
#define MVVM_MODEL_SESSIONMODEL_H

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
  SessionModel(std::string model_type, std::unique_ptr<ItemManagerInterface> manager);

  ~SessionModel() override;
  SessionModel(const SessionModel& other) = delete;
  SessionModel& operator=(const SessionModel& other) = delete;

  // Methods to manipulate data and items.

  SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                          const TagIndex& tag_index) override;

  SessionItem* InsertNewItem(const std::string& item_type, SessionItem* parent = nullptr,
                             const TagIndex& tag_index = {}) final;

  std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) override;

  void RemoveItem(SessionItem* item) override;

  void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index) override;

  variant_t Data(SessionItem* item, int role);

  bool SetData(SessionItem* item, const variant_t& value, int role) override;

  // Various getters.

  std::string GetType() const;

  SessionItem* GetRootItem() const;

  const ItemFactoryInterface* GetFactory() const;

  SessionItem* FindItem(const std::string& id) const;

  template <typename T = SessionItem>
  std::vector<T*> GetTopItems() const;

  template <typename T = SessionItem>
  T* GetTopItem() const;

  // Methods to steer global behaviour.

  void Clear(std::function<void(SessionItem*)> callback = {}) override;

  template <typename T>
  void RegisterItem(const std::string& label = {});

private:
  friend class SessionItem;

  void RegisterInPool(SessionItem* item);
  void UnregisterFromPool(SessionItem* item);

  void RegisterItemInternal(const std::string& item_type, const item_factory_func_t& func,
                            const std::string& label);

  struct SessionModelImpl;
  std::unique_ptr<SessionModelImpl> p_impl;
};

//! Returns top items of the given type.
//! The top item is an item that is a child of an invisible root item.

template <typename T>
std::vector<T*> SessionModel::GetTopItems() const
{
  return utils::CastItems<T>(GetRootItem()->GetAllItems());
}

//! Returns top item of the given type. If more than one item exists, return the first one.
//! The top item is an item that is a child of an invisible root item.

template <typename T>
T* SessionModel::GetTopItem() const
{
  auto items = GetTopItems<T>();
  return items.empty() ? nullptr : items.front();
}

//! Register used defined item to use with the model. It will become possible to undo/redo
//! operations with this item, as well as serialize it to/from XML.

template <typename T>
void SessionModel::RegisterItem(const std::string& label)
{
  RegisterItemInternal(T().GetType(), ItemFactoryFunction<T>, label);
}

}  // namespace mvvm

#endif  // MVVM_MODEL_SESSIONMODEL_H
