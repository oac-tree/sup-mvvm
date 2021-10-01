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

#ifndef MVVM_MODEL_SESSIONMODEL_H
#define MVVM_MODEL_SESSIONMODEL_H

#include "mvvm/core/variant.h"
#include "mvvm/model/function_types.h"
#include "mvvm/model/sessionitem.h"

#include <memory>

namespace ModelView
{
class SessionItem;
class ItemPool;
class ItemFactoryInterface;
class ItemCatalogue;

//! Main class to hold hierarchy of SessionItem objects.

class MVVM_MODEL_EXPORT SessionModel
{
public:
  explicit SessionModel(std::string model_type = {}, std::shared_ptr<ItemPool> pool = {});
  virtual ~SessionModel();
  SessionModel(const SessionModel& other) = delete;
  SessionModel& operator=(const SessionModel& other) = delete;

  // Methods to manipulate data and items.

  SessionItem* InsertNewItem(const std::string& item_type, SessionItem* parent = nullptr,
                             const TagIndex& tag_index = {});

  template <typename T>
  T* InsertItem(SessionItem* parent = nullptr, const TagIndex& tag_index = {});

  void RemoveItem(SessionItem* parent, const TagIndex& tag_index);

  variant_t Data(SessionItem* item, int role);

  bool SetData(SessionItem* item, const variant_t& value, int role);

  // Various getters.

  std::string GetType() const;

  SessionItem* GetRootItem() const;

  const ItemFactoryInterface* GetFactory() const;

  SessionItem* FindItem(const std::string& id);

  template <typename T = SessionItem>
  std::vector<T*> GetTopItems() const;

  template <typename T = SessionItem>
  T* GetTopItem() const;

  // Methods to steer global behaviour.

  void SetItemCatalogue(std::unique_ptr<ItemCatalogue> catalogue);

  void Clear(std::function<void(SessionItem*)> callback = {});

  template <typename T>
  void RegisterItem(const std::string& label = {});

private:
  friend class SessionItem;

  void RegisterInPool(SessionItem* item);
  void UnregisterFromPool(SessionItem* item);

  SessionItem* ItemInsertInternal(const item_factory_func_t& func, SessionItem* parent,
                                  const TagIndex& tag_index);
  void RegisterInPoolInternal(const std::string& item_type, const item_factory_func_t& func,
                              const std::string& label);

  struct SessionModelImpl;
  std::unique_ptr<SessionModelImpl> p_impl;
};

//! Inserts item into given parent under given tag_index.

template <typename T>
T* SessionModel::InsertItem(SessionItem* parent, const TagIndex& tag_index)
{
  return static_cast<T*>(ItemInsertInternal(ItemFactoryFunction<T>(), parent, tag_index));
}

//! Returns top items of the given type.
//! The top item is an item that is a child of an invisible root item.

template <typename T>
std::vector<T*> SessionModel::GetTopItems() const
{
  return Utils::CastItems<T>(GetRootItem()->GetAllItems());
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
//! operations with this item, as well as serialize it to/from JSON.

template <typename T>
void SessionModel::RegisterItem(const std::string& label)
{
  RegisterInPoolInternal(T().GetType(), ItemFactoryFunction<T>(), label);
}

}  // namespace ModelView

#endif  // MVVM_MODEL_SESSIONMODEL_H
