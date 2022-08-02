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

#ifndef MVVM_INTERFACES_SESSIONMODEL_INTERFACE_H_
#define MVVM_INTERFACES_SESSIONMODEL_INTERFACE_H_

#include <mvvm/core/variant.h>
#include <mvvm/interfaces/item_factory_interface.h>
#include <mvvm/model/tagindex.h>
#include <mvvm/model_export.h>

#include <functional>
#include <memory>

namespace mvvm
{
class SessionItem;
class ModelEventSubscriberInterface;

//! Application model interface.

class MVVM_MODEL_EXPORT SessionModelInterface
{
public:
  virtual ~SessionModelInterface() = default;

  virtual std::string GetType() const = 0;

  virtual SessionItem* GetRootItem() const = 0;

  virtual const ItemFactoryInterface* GetFactory() const = 0;

  ItemFactoryInterface* GetFactory();

  virtual ModelEventSubscriberInterface* GetSubscriber() const = 0;

  virtual SessionItem* InsertItem(std::unique_ptr<SessionItem> item, SessionItem* parent,
                                  const TagIndex& tag_index) = 0;

  template <typename T>
  T* InsertItem(SessionItem* parent = nullptr, const TagIndex& tag_index = {});

  virtual std::unique_ptr<SessionItem> TakeItem(SessionItem* parent, const TagIndex& tag_index) = 0;

  virtual void RemoveItem(SessionItem* item) = 0;

  virtual void MoveItem(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index) = 0;

  virtual bool SetData(SessionItem* item, const variant_t& value, int role) = 0;

  virtual SessionItem* FindItem(const std::string& id) const = 0;

  //! Removes all items from the model and recreates a new empty root item.
  //! If `root_item` is provided will use it as a new root.
  virtual void Clear(std::unique_ptr<SessionItem> root_item) = 0;

  virtual void CheckIn(SessionItem* item) = 0;

  virtual void CheckOut(SessionItem* item) = 0;

  template <typename T>
  void RegisterItem(const std::string& label = {});
};

//! Inserts item of given type into given parent under given tag_index.

template <typename T>
T* SessionModelInterface::InsertItem(SessionItem* parent, const TagIndex& tag_index)
{
  return static_cast<T*>(InsertItem(std::make_unique<T>(), parent, tag_index));
}

//! Register used defined item to use with the model. It will become possible to undo/redo
//! operations with this item, as well as serialize it to/from XML.

template <typename T>
void SessionModelInterface::RegisterItem(const std::string& label)
{
  GetFactory()->RegisterItem(T().GetType(), ItemFactoryFunction<T>, label);
}

inline ItemFactoryInterface* SessionModelInterface::GetFactory()
{
  return const_cast<ItemFactoryInterface*>(
        static_cast<const SessionModelInterface*>(this)->GetFactory());
}

}  // namespace mvvm

#endif  // MVVM_INTERFACES_SESSIONMODEL_INTERFACE_H_
