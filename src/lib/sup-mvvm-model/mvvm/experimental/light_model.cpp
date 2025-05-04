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

#include "light_model.h"

#include "light_item.h"

namespace mvvm::experimental
{

LightModel::LightModel() : m_root(std::make_unique<LightItem>()) {}

LightModel::~LightModel() = default;

bool LightModel::SetData(ILightItem *item, const variant_t &value, int32_t role)
{
  // make command
  // notify
  auto result = item->SetDataIntern(value, role);
  // notify
  return result;
}

}  // namespace mvvm::experimental
