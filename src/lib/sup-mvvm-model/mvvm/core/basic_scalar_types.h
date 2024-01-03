/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_CORE_BASIC_SCALAR_TYPES_H_
#define MVVM_CORE_BASIC_SCALAR_TYPES_H_

/**
 * @file
 * Defines basic scalar types supported by the framework. Definitions are taken as it is from
 * sup-dto framework.
 */

#include <cstdint>

namespace mvvm
{

using boolean = bool;
using char8 = char;
using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;
using float32 = float;
using float64 = double;

}  // namespace mvvm

#endif  // MVVM_CORE_BASIC_SCALAR_TYPES_H_
