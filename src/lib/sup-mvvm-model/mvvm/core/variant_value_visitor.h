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

#ifndef MVVM_CORE_VARIANT_VALUE_VISITOR_H_
#define MVVM_CORE_VARIANT_VALUE_VISITOR_H_

#include <mvvm/core/variant.h>

namespace mvvm
{

/**
 * @brief Helper structure to visit variant and report string representation of its value.
 */
struct VariantValueVisitor
{
  std::string operator()(std::monostate value);

  std::string operator()(mvvm::boolean value);

  std::string operator()(mvvm::char8 value);

  std::string operator()(mvvm::int8 value);

  std::string operator()(mvvm::uint8 value);

  std::string operator()(mvvm::int16 value);

  std::string operator()(mvvm::uint16 value);

  std::string operator()(mvvm::int32 value);

  std::string operator()(mvvm::uint32 value);

  std::string operator()(mvvm::int64 value);

  std::string operator()(mvvm::uint64 value);

  std::string operator()(mvvm::float32 value);

  std::string operator()(mvvm::float64 value);

  std::string operator()(std::string value);

  std::string operator()(const std::vector<double> &value);

  std::string operator()(const mvvm::ComboProperty &value);

  std::string operator()(const mvvm::ExternalProperty &value);
};

}  // namespace mvvm

#endif  // MVVM_CORE_VARIANT_VALUE_VISITOR_H_
