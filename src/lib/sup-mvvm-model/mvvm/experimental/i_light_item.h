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

namespace mvvm::experimental
{

class LightModel;

class ILightItem
{
public:
  virtual ~ILightItem() = default;

  virtual bool SetData(const variant_t& value, std::int32_t role) = 0;
  virtual bool SetDataIntern(const variant_t& value, std::int32_t role) = 0;

  virtual LightModel* GetModel() = 0;
};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_I_LIGHT_ITEM_H_
