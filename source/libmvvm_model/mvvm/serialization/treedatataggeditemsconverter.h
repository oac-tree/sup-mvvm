// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_TREEDATATAGGEDITEMSCONVERTE_H
#define MVVM_TREEDATATAGGEDITEMSCONVERTE_H

//! @file treedatataggeditemsconverter.h
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

#include "mvvm/model_export.h"
#include "mvvm/serialization/converter_types.h"

#include <memory>

namespace ModelView
{
class TreeData;
class TaggedItems;

//! Convert TaggedItems to/from TreeData objects.

class MVVM_MODEL_EXPORT TreeDataTaggedItemsConverter
{
public:
  TreeDataTaggedItemsConverter(const ConverterCallbacks& callbacks);
  ~TreeDataTaggedItemsConverter();

  //! Returns true if given TreeData can be converted to TaggedItems
  bool IsTaggedItemsConvertible(const TreeData& tree_data) const;

  //! Creates TaggedItems from TreeData object.
  std::unique_ptr<TaggedItems> ToTaggedItems(const TreeData& tree_data) const;

  //! Populates existing TaggedItems from TreeData object.
  void PopulateTaggedItems(const TreeData& tree_data, TaggedItems& tagged_items) const;

  //! Creates TreeData from TaggedItems object.
  std::unique_ptr<TreeData> ToTreeData(const TaggedItems& tagged_items) const;

private:
  struct TreeDataTaggedItemsConverterImpl;
  std::unique_ptr<TreeDataTaggedItemsConverterImpl> p_impl;
};

}  // namespace ModelView

#endif  // MVVM_TREEDATATAGINFOTCONVERTER_H
