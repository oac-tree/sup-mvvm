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

#include "mvvm/experimental/abstract_model_decorator.h"

#include "mvvm/experimental/item.h"

namespace mvvm::experimental
{

AbstractModelDecorator::AbstractModelDecorator(std::unique_ptr<ModelInterface> decorated_model)
    : m_decorated_model(std::move(decorated_model))
{
}

Item *AbstractModelDecorator::GetRootItem() const
{
  return m_decorated_model->GetRootItem();
}

bool AbstractModelDecorator::SetData(Item *item, const variant_t &data)
{
  return m_decorated_model->SetData(item, data);
}

Item *AbstractModelDecorator::InsertItem(std::unique_ptr<Item> item, Item *parent, int index)
{
  return m_decorated_model->InsertItem(std::move(item), parent, index);
}

std::unique_ptr<Item> AbstractModelDecorator::TakeItem(Item *parent, int index)
{
  return m_decorated_model->TakeItem(parent, index);
}

}  // namespace mvvm::experimental
