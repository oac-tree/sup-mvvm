/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_CORE_VARIANT_VISITORS_H_
#define MVVM_CORE_VARIANT_VISITORS_H_

//! @file
//! Defines visitors for variant.

#include <mvvm/core/exceptions.h>
#include <mvvm/core/variant.h>

namespace mvvm
{

/**
 * @brief Helper structure to visit variant and report numeric limits of stored type.
 *
 * @details Used in the context of integer editors. It is implemented only for types which we can
 * fit into int32: int8, uint8, int16, uint16, int32.
 */

struct VariantLimits32Visitor
{
  std::pair<int, int> operator()(std::monostate);

  std::pair<int, int> operator()(mvvm::boolean value);

  std::pair<int, int> operator()(mvvm::char8 value);

  std::pair<int, int> operator()(mvvm::int8 value);

  std::pair<int, int> operator()(mvvm::uint8 value);

  std::pair<int, int> operator()(mvvm::int16 value);

  std::pair<int, int> operator()(mvvm::uint16 value);

  std::pair<int, int> operator()(mvvm::int32 value);

  std::pair<int, int> operator()(mvvm::uint32 value);

  std::pair<int, int> operator()(mvvm::int64 value);

  std::pair<int, int> operator()(mvvm::uint64 value);

  std::pair<int, int> operator()(mvvm::float32 value);

  std::pair<int, int> operator()(mvvm::float64 value);

  std::pair<int, int> operator()(std::string value);

  std::pair<int, int> operator()(std::vector<double> value);

  std::pair<int, int> operator()(mvvm::ComboProperty value);

  std::pair<int, int> operator()(mvvm::ExternalProperty value);

  std::pair<int, int> operator()(mvvm::Limits<int> value);

  std::pair<int, int> operator()(mvvm::Limits<mvvm::int64> value);

  std::pair<int, int> operator()(mvvm::Limits<double> value);
};

/**
 * @brief Returns pairs of integers representing numeric limits of stored type.
 *
 * @details Used in the context of integer editors. It is implemented only for types which we can
 * fit into int32: int8, uint8, int16, uint16, int32.
 */
std::pair<int, int> GetInt32MinMaxNumeric(const variant_t& value);


} // namespace mvvm

#endif  // MVVM_CORE_VARIANT_VISITORS_H_
