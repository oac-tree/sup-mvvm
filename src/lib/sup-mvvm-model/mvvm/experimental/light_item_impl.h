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

#ifndef MVVM_EXPERIMENTAL_LIGHT_ITEM_IMPL_H_
#define MVVM_EXPERIMENTAL_LIGHT_ITEM_IMPL_H_

#include <mvvm/experimental/i_light_item.h>

#include <memory>

namespace mvvm
{
class SessionItemData;
}  // namespace mvvm

namespace mvvm::experimental
{

class LightModel;

class LightItemImpl : public ILightItem
{
public:
  LightItemImpl();
  ~LightItemImpl() override;

  bool SetData(const variant_t& value, std::int32_t role) override;
  bool SetDataIntern(const variant_t& value, std::int32_t role) override;

  LightModel* GetModel() override;

private:
  std::unique_ptr<SessionItemData> m_data;
  LightModel* m_model{nullptr};
};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_LIGHT_ITEM_IMPL_H_
