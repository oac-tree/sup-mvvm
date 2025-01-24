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

#ifndef MVVM_SERIALIZATION_TREEDATA_TAGINFO_CONVERTER_H_
#define MVVM_SERIALIZATION_TREEDATA_TAGINFO_CONVERTER_H_

//! @file
//! Declares utility functions to convert between TreeData and TagInfo objects.

//! These are auxiliary functions to provide parsing of xml elements
//! - <TagInfo min="0" max="1" name="TagName">SegmentItem</Variant>

#include <mvvm/model_export.h>

#include <mvvm/serialization/tree_data_fwd.h>

namespace mvvm
{

class TagInfo;

//! Returns true if given TreeData represents TagInfo object.
MVVM_MODEL_EXPORT bool IsTagInfoConvertible(const tree_data_t& tree_data);

//! Returns TagInfo object from its TreeData representation. Will throw if convertions is not
//! possible.
MVVM_MODEL_EXPORT TagInfo ToTagInfo(const tree_data_t& tree_data);

//! Returns TreeData object constructed from TagInfo object.
MVVM_MODEL_EXPORT tree_data_t ToTreeData(const TagInfo& tag_info);

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREEDATA_TAGINFO_CONVERTER_H_
