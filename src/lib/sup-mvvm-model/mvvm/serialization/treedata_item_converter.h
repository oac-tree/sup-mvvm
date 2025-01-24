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

#ifndef MVVM_SERIALIZATION_TREEDATA_ITEM_CONVERTER_H_
#define MVVM_SERIALIZATION_TREEDATA_ITEM_CONVERTER_H_

#include <mvvm/serialization/converter_types.h>
#include <mvvm/serialization/treedata_item_converter_interface.h>

namespace mvvm
{

class IItemFactory;

//! Converters SessionItem to/from TreeData object.
//! Requires ItemFactory to operate. On the way from SessionItem to TreeData creates
//! exact copy of the data. On the way back, creates full clone of original item (including
//! unique identifiers), if ConverterMode::kClone.

class MVVM_MODEL_EXPORT TreeDataItemConverter : public TreeDataItemConverterInterface
{
public:
  TreeDataItemConverter(const IItemFactory* factory, ConverterMode mode);
  ~TreeDataItemConverter() override;

  //! Returns true if given TreeData represents SessionItem object.
  bool IsSessionItemConvertible(const tree_data_t& tree_data) const;

  //! Creates SessionItem from TreeData.
  std::unique_ptr<SessionItem> ToSessionItem(const tree_data_t& tree_data) const override;

  //! Creates TreeData from SessionItem.
  std::unique_ptr<TreeData> ToTreeData(const SessionItem& item) const override;

private:
  struct TreeDataItemConverterImpl;
  std::unique_ptr<TreeDataItemConverterImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREEDATA_ITEM_CONVERTER_H_
