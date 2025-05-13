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

#include <stdexcept>

namespace mvvm::experimental
{

LightItemImpl::LightItemImpl() : ILightItem(), m_data(std::make_unique<SessionItemData>()) {}

LightItemImpl::~LightItemImpl() = default;

bool LightItemImpl::SetData(const variant_t &value, int32_t role)
{
  return m_data->SetData(value, role);
}

bool LightItemImpl::SetDataIntern(const variant_t &value, int32_t role)
{
  return m_data->SetData(value, role);
}

bool LightItemImpl::SetDataImpl(const variant_t &value, int32_t role)
{
  (void)value;
  (void)role;
  throw std::runtime_error("Should not be called");
}

variant_t LightItemImpl::Data(int32_t role) const
{
  return m_data->Data(role);
}

ILightItem *LightItemImpl::InsertItem(std::unique_ptr<ILightItem> item, std::size_t index)
{
  auto result = item.get();
  m_container.insert(std::next(m_container.begin(), index), std::move(item));
  result->SetParent(this);
  result->SetModel(GetModel());
  return result;
}

void LightItemImpl::SetModel(LightModel *model)
{
  m_model = model;
  for (auto &child : m_container)
  {
    child->SetModel(m_model);
  }
}

LightModel *LightItemImpl::GetModel()
{
  return m_model;
}

void LightItemImpl::SetParent(ILightItem *parent)
{
  m_parent = parent;
}

}  // namespace mvvm::experimental
