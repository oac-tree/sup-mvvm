/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_SERIALIZATION_TREEDATA_ITEMDATA_CONVERTER_H_
#define MVVM_SERIALIZATION_TREEDATA_ITEMDATA_CONVERTER_H_

//! @file treedataitemdataconverter.h
//! Declares class TreeDataItemDataConverter to convert SessionItemData
//! to/from TreeData objects.

//! Plays supporting role in converting XML elements
//! <ItemData>
//!   <Variant role="0" type="int">42</Variant>
//!   <Variant role="1" type="vector_double_t">42.1, 42.2, 42.3</Variant>
//! </ItemData>

#include <mvvm/model_export.h>

#include <memory>

namespace mvvm
{
class TreeData;
class SessionItemData;

//! Convert SessionItemData to/from TreeData objects.

//! TODO For the moment convertion is performed for every datarole_t found.
//! In the future the filtering of roles should be implemented, as in original qt-mvm.
class MVVM_MODEL_EXPORT TreeDataItemDataConverter
{
public:
  TreeDataItemDataConverter();

  //! Converts SessionItemData to TreeData.
  std::unique_ptr<TreeData> ToTreeData(const SessionItemData& item_data) const;

  //! Creates SessionItemData from TreeData.
  std::unique_ptr<SessionItemData> ToSessionItemData(const TreeData& tree_data) const;

  //! Populates existing SessionItemData from TreeData object.
  void PopulateItemData(const TreeData& tree_data, SessionItemData& item_data) const;

  bool IsSessionItemDataConvertible(const TreeData& tree_data) const;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREEDATA_ITEMDATA_CONVERTER_H_
