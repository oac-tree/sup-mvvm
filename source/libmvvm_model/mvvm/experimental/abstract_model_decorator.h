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

#ifndef MVVM_MODEL_ABSTRACT_MODEL_DECORATOR_H_
#define MVVM_MODEL_ABSTRACT_MODEL_DECORATOR_H_

#include <mvvm/experimental/model_interface.h>

namespace mvvm::experimental
{

class Item;

class AbstractModelDecorator : public ModelInterface
{
public:
  explicit AbstractModelDecorator(std::unique_ptr<ModelInterface> decorated_model);

  Item* GetRootItem() const override;

  bool SetData(Item* item, const variant_t& data) override;

  Item* InsertItem(std::unique_ptr<Item> item, Item* parent, int index) override;

  std::unique_ptr<Item> TakeItem(Item* parent, int index) override;

private:
  std::unique_ptr<ModelInterface> m_decorated_model;
};

}  // namespace mvvm::experimental

#endif  // MVVM_MODEL_ABSTRACT_MODEL_DECORATOR_H_
