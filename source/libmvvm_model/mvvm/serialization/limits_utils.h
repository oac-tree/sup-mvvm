/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_SERIALIZATION_LIMITS_UTILS_H_
#define MVVM_SERIALIZATION_LIMITS_UTILS_H_

//! @file limits_utils.h
//! Utility functions to convert limits to string. Used to serialize Limits in XML.

#include <mvvm/model_export.h>

#include <string>

namespace mvvm
{
template <typename T>
class Limits;
}

namespace mvvm::utils
{

//! Returns string representing Limits<int>.
//! @note Limited(1, 2) -> "limited;1;2"
//! - Positive() -> "positive"
//! - UpperLimited(42) -> "upperlimited;42"
std::string ToString(const Limits<int>& limits);

//! Returns string representing Limits<double>.
//! @note Limited(1.0, 2.0) -> "limited;1.0;2.0"
//! - Positive() -> "positive"
//! - UpperLimited(42.1) -> "upperlimited;42.1"
std::string ToString(const Limits<double>& limits);

}  // namespace mvvm::utils

#endif  // MVVM_SERIALIZATION_LIMITS_UTILS_H_
