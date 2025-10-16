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

#include "bin_utils.h"

namespace mvvm::utils
{

bool IsBinaryFile(const std::string &file_name)
{
  (void)file_name;
  return false;
}

bool IsTextFile(const std::string &file_name)
{
  return !IsBinaryFile(file_name);
}

}  // namespace mvvm::utils
