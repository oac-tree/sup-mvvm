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

#ifndef MVVM_SERIALIZATION_TREE_DATA_TAGGED_ITEMS_CONVERTER_H_
#define MVVM_SERIALIZATION_TREE_DATA_TAGGED_ITEMS_CONVERTER_H_

//! @file
//! Declares class TreeDataTaggedItemsConverter to convert TaggedItems
//! to/from TreeData objects.

//! Plays supporting role in converting XML elements
//!
//! <TaggedItems defaultTag="defaultTag">
//!   <ItemContainer>
//!     <!--Here goes container content-->
//!   </ItemContainer>
//!   <ItemContainer>
//!     <!--Here goes container content-->
//!   </ItemContainer>
//! </TaggedItems>

#include <mvvm/model_export.h>
#include <mvvm/serialization/converter_types.h>

#include <memory>

namespace mvvm
{

class TaggedItems;

//! Convert TaggedItems to/from TreeData objects.

class MVVM_MODEL_EXPORT TreeDataTaggedItemsConverter
{
public:
  TreeDataTaggedItemsConverter(const ConverterCallbacks& callbacks);
  ~TreeDataTaggedItemsConverter();

  //! Returns true if given TreeData can be converted to TaggedItems
  bool IsTaggedItemsConvertible(const tree_data_t& tree_data) const;

  //! Creates TaggedItems from TreeData object.
  std::unique_ptr<TaggedItems> ToTaggedItems(const tree_data_t& tree_data) const;

  //! Populates existing TaggedItems from TreeData object.
  void PopulateTaggedItems(const tree_data_t& tree_data, TaggedItems& tagged_items) const;

  //! Creates TreeData from TaggedItems object.
  std::unique_ptr<tree_data_t> ToTreeData(const TaggedItems& tagged_items) const;

private:
  struct TreeDataTaggedItemsConverterImpl;
  std::unique_ptr<TreeDataTaggedItemsConverterImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREE_DATA_TAGGED_ITEMS_CONVERTER_H_
