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

#include "light_item_impl.h"

#include "light_model.h"
#include "mvvm/model/session_item_data.h"

namespace mvvm::experimental
{

LightItemImpl::LightItemImpl() : ILightItem(), m_data(std::make_unique<SessionItemData>()) {}

LightItemImpl::~LightItemImpl() = default;

bool LightItemImpl::SetData(const variant_t &value, int32_t role)
{
  return m_data->SetData(value, role);
}

LightModel *LightItemImpl::GetModel()
{
  return m_model;
}

}  // namespace mvvm::experimental
