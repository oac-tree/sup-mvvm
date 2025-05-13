/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "light_item.h"

#include "light_item_impl.h"
#include "light_model.h"
#include "light_set_data_command.h"

namespace mvvm::experimental
{

LightItem::LightItem() : ILightItem(), m_impl(std::make_unique<LightItemImpl>())
{
  auto set_data_strategy = [](auto item, const auto &value, auto role)
  { return item->SetDataIntern(value, role); };
  m_set_data_strategy = set_data_strategy;
}

void LightItem::SetDataStrategy(set_data_strategy_t strategy)
{
  m_set_data_strategy = std::move(strategy);
}

LightItem::~LightItem() = default;

bool LightItem::SetData(const variant_t &value, int32_t role)
{
  return m_set_data_strategy(this, value, role);
}

bool LightItem::SetDataIntern(const variant_t &value, int32_t role)
{
  if (auto model = GetModel(); model)
  {
    auto command = std::make_unique<LightSetDataCommand>(this, value, role);
    return model->ExecuteCommand(std::move(command));
  }

  return m_impl->SetData(value, role);
}

variant_t LightItem::Data(int32_t role) const
{
  return m_impl->Data(role);
}

ILightItem *LightItem::InsertItem(std::unique_ptr<ILightItem> item, std::size_t index)
{
  return m_impl->InsertItem(std::move(item), index);
}

void LightItem::SetModel(LightModel *model)
{
  m_impl->SetModel(model);
}

LightModel *LightItem::GetModel()
{
  return m_impl->GetModel();
}

void LightItem::SetParent(ILightItem *parent)
{
  m_impl->SetParent(parent);
}

}  // namespace mvvm::experimental
