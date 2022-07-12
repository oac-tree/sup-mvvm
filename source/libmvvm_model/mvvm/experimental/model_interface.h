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

#ifndef MVVM_MODEL_EXPERIMENTAL_MODEL_INTERFACE_H_
#define MVVM_MODEL_EXPERIMENTAL_MODEL_INTERFACE_H_

#include <mvvm/core/variant.h>

#include <memory>

namespace mvvm::experimental
{

class Item;

class ModelInterface
{
public:
  virtual ~ModelInterface() = default;

  virtual Item* GetRootItem() const = 0;

  virtual bool SetData(Item* item, const variant_t& data) = 0;

  virtual variant_t GetData(Item* item) const = 0;

  virtual Item* InsertItem(std::unique_ptr<Item> item, Item* parent, int index) = 0;

  template <typename T>
  T* InsertItem(Item* parent, int index);

  virtual std::unique_ptr<Item> TakeItem(Item* parent, int index) = 0;
};

template <typename T>
T* ModelInterface::InsertItem(Item* parent, int index)
{
  return static_cast<T*>(InsertItem(std::make_unique<T>(), parent, index));
}

}  // namespace mvvm::experimental

#endif  // MVVM_MODEL_EXPERIMENTAL_MODEL_INTERFACE_H_
