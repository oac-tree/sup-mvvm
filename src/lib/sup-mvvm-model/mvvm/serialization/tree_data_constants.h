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

namespace mvvm::constants
{

constexpr auto kRootElementType = "MvvmDocument";
constexpr auto kApplicationTypeAttribute = "application";
constexpr auto kModelElementType = "Model";
constexpr auto kItemElementType = "Item";
constexpr auto kTaggedItemsElementType = "TaggedItems";
constexpr auto kItemDataElementType = "ItemData";
constexpr auto kDefaultTagKey = "defaultTag";
constexpr auto kItemContainerElementType = "ItemContainer";
constexpr auto kTagInfoElementType = "TagInfo";
constexpr auto kVariantElementType = "Variant";
constexpr auto kRoleAttributeKey = "role";
constexpr auto kTypeAttributeKey = "type";
constexpr auto kMinAttributeKey = "min";
constexpr auto kMaxAttributeKey = "max";
constexpr auto kNameAttributeKey = "name";

}  // namespace mvvm::constants

#endif  // MVVM_SERIALIZATION_TREE_DATA_CONSTANTS_H_
