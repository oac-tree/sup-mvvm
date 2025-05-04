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

namespace mvvm::experimental
{

LightItem::LightItem() : ILightItem(), m_impl(std::make_unique<LightItemImpl>())
{
  // auto set_data_strategy = [](auto item, auto value, auto role)
  // {
  //   item->SetData(value, role);
  // };
}

void LightItem::SetDataStrategy(set_data_strategy_t strategy)
{
  m_set_data_strategy = std::move(strategy);
}

LightItem::~LightItem() = default;

bool LightItem::SetData(const variant_t &value, int32_t role)
{
  if (auto model = GetModel(); model)
  {
    return model->SetData(this, value, role);
  }

  return SetDataIntern(value, role);
}

bool LightItem::SetDataIntern(const variant_t &value, int32_t role)
{
  return m_impl->SetData(value, role);
}

LightModel *LightItem::GetModel()
{
  return m_impl->GetModel();
}

}  // namespace mvvm::experimental
