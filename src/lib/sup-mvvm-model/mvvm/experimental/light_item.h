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

#include <functional>
#include <memory>

namespace mvvm::experimental
{

class LightModel;
class LightItemImpl;

class LightItem : public ILightItem
{
public:
  using set_data_strategy_t =
      std::function<bool(LightItem*, const variant_t& value, std::int32_t role)>;

  LightItem();
  ~LightItem() override;

  void SetDataStrategy(set_data_strategy_t strategy);

  bool SetData(const variant_t& value, std::int32_t role = DataRole::kData) override;

  bool SetDataIntern(const variant_t& value, std::int32_t role) override;

  variant_t Data(std::int32_t role = DataRole::kData) const override;

  ILightItem* InsertItem(std::unique_ptr<ILightItem> item, std::size_t index) override;

  void SetModel(LightModel* model) override;

  LightModel* GetModel() override;

  ILightItem* GetParent() override;

  void SetParent(ILightItem* parent) override;

  ILightItem* GetItem(std::size_t index) override;

private:
  std::unique_ptr<LightItemImpl> m_impl;
  set_data_strategy_t m_set_data_strategy;
};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_LIGHT_ITEM_H_
