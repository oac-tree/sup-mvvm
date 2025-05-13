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

#ifndef MVVM_EXPERIMENTAL_I_LIGHT_ITEM_H_
#define MVVM_EXPERIMENTAL_I_LIGHT_ITEM_H_

#include <mvvm/core/variant.h>
#include <mvvm/model/mvvm_types.h>
#include <memory>

namespace mvvm::experimental
{

class LightModel;

class ILightItem
{
public:
  virtual ~ILightItem() = default;

  virtual bool SetData(const variant_t& value, std::int32_t role = DataRole::kData) = 0;

  virtual bool SetDataIntern(const variant_t& value, std::int32_t role) = 0;

  virtual bool SetDataImpl(const variant_t& value, std::int32_t role) = 0;

  virtual variant_t Data(std::int32_t role = DataRole::kData) const = 0;

  virtual ILightItem* InsertItem(std::unique_ptr<ILightItem> item, std::size_t index) = 0;

  virtual void SetModel(LightModel* model) = 0;

  virtual LightModel* GetModel() = 0;

  virtual ILightItem* GetParent() = 0;

  virtual void SetParent(ILightItem* parent) = 0;

  virtual ILightItem* GetItem(std::size_t index) = 0;
};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_I_LIGHT_ITEM_H_
