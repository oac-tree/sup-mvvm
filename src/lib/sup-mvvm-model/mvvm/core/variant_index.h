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

#ifndef MVVM_CORE_VARIANT_INDEX_H_
#define MVVM_CORE_VARIANT_INDEX_H_

//! @file
//! Compile-time helper functions for variant.

#include <variant>

namespace mvvm
{

/**
 * @brief Returns compile time index of type stored in given variant type.
 *
 *  https://gist.github.com/nnaumenko/1db96f7e187979a057ee7ad757dee4f2
 *
 * @tparam VariantT Variant type to search for index.
 * @tparam ContentT One of variant underlying types to search for index.
 * @return Index of ContentT in VariantT, or variant size if type is not present.
 */
template <typename VariantT, typename ContentT, std::size_t I = 0>
constexpr std::size_t variant_index()
{
  if constexpr (I >= std::variant_size_v<VariantT>)
  {
    return (std::variant_size_v<VariantT>);
  }
  else
  {
    if constexpr (std::is_same_v<std::variant_alternative_t<I, VariantT>, ContentT>)
    {
      return (I);
    }
    else
    {
      return (variant_index<VariantT, ContentT, I + 1>());
    }
  }
}

}  // namespace mvvm

#endif  // MVVM_CORE_VARIANT_H_
