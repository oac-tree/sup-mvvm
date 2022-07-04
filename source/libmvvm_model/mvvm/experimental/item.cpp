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

#include "mvvm/experimental/item.h"

#include <stdexcept>

namespace mvvm::experimental
{

Item::Item() {}

int Item::GetItemCount() const
{
  return static_cast<int>(m_items.size());
}

Item *Item::GetParent() const
{
  return m_parent;
}

void Item::SetParent(Item *parent)
{
  m_parent = parent;
}

Model *Item::GetModel() const
{
  return m_model;
}

void Item::SetModel(Model *model)
{
  m_model = model;
}

Item *Item::GetItem(int index)
{
  return index >= 0 && index < GetItemCount() ? m_items[index].get() : nullptr;
}

Item *Item::InsertItem(std::unique_ptr<Item> item, int index)
{
  if (index < 0 || index > GetItemCount())
  {
    throw std::runtime_error("Wrong index");
  }

  auto result = item.get();

  item->SetParent(this);
  item->SetModel(GetModel());

  m_items.emplace(std::next(m_items.begin(), index), std::move(item));

  return result;
}

std::unique_ptr<Item> Item::TakeItem(int index)
{
  if (index < 0 || index >= GetItemCount())
  {
    throw std::runtime_error("Wrong index");
  }

  auto result = std::move(m_items[index]);
  m_items.erase(std::next(m_items.begin(), index));
  return result;
}

variant_t Item::GetData() const
{
  return m_data;
}

bool Item::SetData(const variant_t &data)
{
  if (m_data != data)
  {
    m_data = data;
    return true;
  }

  return false;
}

}  // namespace mvvm::experimental
