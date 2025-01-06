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

#ifndef MVVM_MODEL_ITEM_CONSTANTS_H_
#define MVVM_MODEL_ITEM_CONSTANTS_H_

//!< @file
//! Common constants for items.

#include <string>

namespace mvvm::constants
{

const std::string kRootItemName = "RootItem";
const std::string kRootItemTag = "RootTag";

const std::string kChildrenTag = "kChildrenTag";
const std::string kLink = "kLink";

const std::string kX = "kX";
const std::string kY = "kY";
const std::string kZ = "kZ";

const std::string kPen = "kPen";
const std::string kDisplayed = "kDisplayed";

}  // namespace mvvm::constants

#endif  // MVVM_MODEL_ITEM_CONSTANTS_H_
