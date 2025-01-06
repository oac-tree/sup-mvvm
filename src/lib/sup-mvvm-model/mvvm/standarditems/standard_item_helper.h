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

#ifndef MVVM_STANDARDITEMS_STANDARD_ITEM_HELPER_H_
#define MVVM_STANDARDITEMS_STANDARD_ITEM_HELPER_H_

//! @file
//! Collection of helper methods to construct standard items.

#include <string>

namespace mvvm
{

/**
 * @brief Returns the default size for axis title.
 */
int GetDefaultAxisTitlePointSize();

/**
 * @brief Returns the default font family for axis title.
 */
std::string GetDefaultAxisFontFamily();

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_STANDARD_ITEM_HELPER_H_
