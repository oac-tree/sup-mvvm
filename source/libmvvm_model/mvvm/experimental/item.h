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

#ifndef MVVM_MODEL_EXPERIMENTAL_ITEM_H_
#define MVVM_MODEL_EXPERIMENTAL_ITEM_H_

#include <mvvm/core/variant.h>

#include <memory>
#include <vector>

namespace mvvm::experimental
{

class Model;

class Item
{
public:
  Item();

  int GetItemCount() const;

  Item* GetParent() const;
  void SetParent(Item* parent);

  Model* GetModel() const;
  void SetModel(Model* model);

  Item* GetItem(int index);

  Item* InsertItem(std::unique_ptr<Item> item, int index);

  std::unique_ptr<Item> TakeItem(int index);

  variant_t GetData() const;

  bool SetData(const variant_t& data);

private:
  std::vector<std::unique_ptr<Item>> m_items;
  Item* m_parent{nullptr};
  Model* m_model{nullptr};
  variant_t m_data;
};

}  // namespace mvvm::experimental

#endif  // MVVM_MODEL_EXPERIMENTAL_ITEM_H_
