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

#ifndef MVVM_MODEL_EXPERIMENTAL_MODEL_H_
#define MVVM_MODEL_EXPERIMENTAL_MODEL_H_

#include <mvvm/core/variant.h>
#include <mvvm/experimental/model_interface.h>

#include <string>

namespace mvvm::experimental
{

class Item;

class Model : public ModelInterface
{
public:
  Model(const std::string& model_type = {});
  ~Model() override;

  Item* GetRootItem() const override;

  bool SetData(Item* item, const variant_t& data);

  Item* InsertItem(std::unique_ptr<Item> item, Item* parent, int index) override;

  std::unique_ptr<Item> TakeItem(Item* parent, int index) override;

private:
  std::string m_model_type;
  std::unique_ptr<Item> m_root_item;
};

}  // namespace mvvm::experimental

#endif  // MVVM_MODEL_EXPERIMENTAL_MODEL_H_
