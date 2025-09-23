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

#ifndef MVVM_SERIALIZATION_TREE_DATA_TAGINFO_CONVERTER_H_
#define MVVM_SERIALIZATION_TREE_DATA_TAGINFO_CONVERTER_H_

//! @file
//! Declares utility functions to convert between TreeData and TagInfo objects.

//! These are auxiliary functions to provide parsing of xml elements
//! - <TagInfo min="0" max="1" name="TagName">SegmentItem</Variant>

#include <mvvm/serialization/tree_data_fwd.h>

namespace mvvm
{

class TagInfo;

namespace TreeDataTagInfoConverter
{

//! Returns true if given TreeData represents TagInfo object.
bool IsTagInfoConvertible(const tree_data_t& tree_data);

//! Returns TagInfo object from its TreeData representation. Will throw if conversion is not
//! possible.
TagInfo ToTagInfo(const tree_data_t& tree_data);

//! Returns TreeData object constructed from TagInfo object.
tree_data_t ToTreeData(const TagInfo& tag_info);

}  // namespace TreeDataTagInfoConverter
}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREE_DATA_TAGINFO_CONVERTER_H_
