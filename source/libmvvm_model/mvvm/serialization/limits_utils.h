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
//! @note Limits<int>::CreateLimited(1, 2) -> "limited;1;2"
//! - Limits<int>::CreatePositive() -> "positive"
//! - Limits<int>::CreateUpperLimited(42) -> "upperlimited;42"
std::string ToString(const Limits<int>& limits);

//! Returns Limits<int> from it's string representation
//! @note "limited(1,2) -> is equivalent of Limits::CreateLimited(1,2)
Limits<int> IntLimitsFromString(const std::string& str);

//! Returns string representing Limits<double>.
//! @note Limits<double>::CreateLimited(1.0, 2.0) -> "limited;1.0;2.0"
//! - Limits<double>::CreatePositive() -> "positive"
//! - Limits<double>::CreateUpperLimited(42.1) -> "upperlimited;42.1"
std::string ToString(const Limits<double>& limits);

//! Returns Limits<int> from it's string representation
//! @note "limited(1,2) -> is equivalent of Limits::CreateLimited(1,2)
Limits<double> RealLimitsFromString(const std::string& str);

}  // namespace mvvm::utils

#endif  // MVVM_SERIALIZATION_LIMITS_UTILS_H_
