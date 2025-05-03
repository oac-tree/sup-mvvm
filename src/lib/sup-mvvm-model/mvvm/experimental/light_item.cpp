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
#include "light_model.h"

#include "mvvm/model/session_item_data.h"

namespace mvvm::experimental
{

LightItem::LightItem() : m_data(std::make_unique<SessionItemData>()) {}

LightItem::~LightItem() = default;

bool LightItem::SetData(const variant_t &value, int32_t role)
{
  if (auto model = GetModel(); model)
  {
    return m_model->SetData(this, value, role);
  }
  return m_data->SetData(value, role);
}

LightModel *LightItem::GetModel()
{
  return m_model;
}

}  // namespace mvvm::experimental
