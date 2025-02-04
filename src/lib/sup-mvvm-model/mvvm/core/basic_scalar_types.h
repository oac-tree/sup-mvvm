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

namespace mvvm
{

using boolean = bool;
using char8 = char;
using int8 = signed char;
using uint8 = unsigned char;
using int16 = signed short;
using uint16 = unsigned short;
using int32 = signed int;
using uint32 = unsigned int;
using int64 = signed long;
using uint64 = unsigned long;
using float32 = float;
using float64 = double;

}  // namespace mvvm

#endif  // MVVM_CORE_BASIC_SCALAR_TYPES_H_
