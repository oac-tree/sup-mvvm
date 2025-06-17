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

#ifndef MVVM_SERIALIZATION_TREE_DATA_CONSTANTS_H_
#define MVVM_SERIALIZATION_TREE_DATA_CONSTANTS_H_

#include <string>

namespace mvvm::constants
{

const std::string kItemElementType = "Item";
const std::string kTypelAttributeKey = "type";
const std::string kTaggedItemsElementType = "TaggedItems";
const std::string kItemDataElementType = "ItemData";
const std::string kDefaultTagKey = "defaultTag";
const std::string kItemContainerElementType = "ItemContainer";
const std::string kTagInfoElementType = "TagInfo";

}  // namespace mvvm::constants

#endif
