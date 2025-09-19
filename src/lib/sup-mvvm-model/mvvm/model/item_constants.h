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

#ifndef MVVM_MODEL_ITEM_CONSTANTS_H_
#define MVVM_MODEL_ITEM_CONSTANTS_H_

//!< @file
//! Common constants for items.

#include <string_view>

namespace mvvm::constants
{

constexpr std::string_view kRootItemName = "RootItem";
constexpr std::string_view kRootItemTag = "RootTag";

constexpr std::string_view kChildrenTag = "kChildrenTag";
constexpr std::string_view kLink = "kLink";

constexpr std::string_view kX = "kX";
constexpr std::string_view kY = "kY";
constexpr std::string_view kZ = "kZ";

constexpr std::string_view kPen = "kPen";
constexpr std::string_view kDisplayed = "kDisplayed";

}  // namespace mvvm::constants

#endif  // MVVM_MODEL_ITEM_CONSTANTS_H_
