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

#ifndef MVVM_EXPERIMENTAL_LIGHT_MODEL_H_
#define MVVM_EXPERIMENTAL_LIGHT_MODEL_H_

#include <mvvm/core/variant.h>

#include <memory>

namespace mvvm::experimental
{

class LightItem;

class LightModel
{
public:
  LightModel();
  ~LightModel();

  bool SetData(LightItem* item, const variant_t& value, std::int32_t role);

private:
  std::unique_ptr<LightItem> m_root;
};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_LIGHT_MODEL_H_
