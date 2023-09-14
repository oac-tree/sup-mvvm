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

#include "basic_scalar_types.h"

#include <climits>

namespace mvvm
{

static_assert(sizeof(int64) == 8, "int64 type needs to have size 8 bytes");
static_assert(sizeof(uint64) == 8, "uint64 type needs to have size 8 bytes");
static_assert(sizeof(float64) == 8, "float64 type needs to have size 8 bytes");

}
