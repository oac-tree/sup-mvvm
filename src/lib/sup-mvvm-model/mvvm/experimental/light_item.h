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

#ifndef MVVM_EXPERIMENTAL_LIGHT_ITEM_H_
#define MVVM_EXPERIMENTAL_LIGHT_ITEM_H_

#include <mvvm/experimental/i_light_item.h>

#include <memory>
#include <functional>

namespace mvvm
{
class SessionItemData;
}  // namespace mvvm

namespace mvvm::experimental
{

class LightModel;
class LightItemImpl;

class LightItem : public ILightItem
{
public:
  LightItem();
  ~LightItem() override;

  bool SetData(const variant_t& value, std::int32_t role) override;

  LightModel* GetModel() override;

private:
  std::unique_ptr<LightItemImpl> m_impl;
  std::function<bool(const variant_t& value, std::int32_t role)> m_set_data_strategy;
};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_LIGHT_ITEM_H_
