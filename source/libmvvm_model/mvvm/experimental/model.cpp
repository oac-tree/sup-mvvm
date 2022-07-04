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

#include "mvvm/experimental/model.h"

#include <mvvm/experimental/item.h>

namespace mvvm::experimental
{

Model::Model() : m_root_item(std::make_unique<Item>()) {}

Model::~Model() = default;

Item *Model::GetRootItem() const
{
  return m_root_item.get();
}

Item *Model::InsertItem(std::unique_ptr<Item> item, Item *parent, int index)
{
  return parent->InsertItem(std::move(item), index);
}

std::unique_ptr<Item> Model::TakeItem(Item *parent, int index)
{
  return parent->TakeItem(index);
}

}  // namespace mvvm::experimental
