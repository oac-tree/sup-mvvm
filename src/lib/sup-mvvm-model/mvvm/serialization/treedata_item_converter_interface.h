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

#ifndef MVVM_SERIALIZATION_TREEDATA_ITEM_CONVERTER_INTERFACE_H_
#define MVVM_SERIALIZATION_TREEDATA_ITEM_CONVERTER_INTERFACE_H_

#include <mvvm/model_export.h>
#include <mvvm/serialization/tree_data_fwd.h>

#include <memory>

namespace mvvm
{

class SessionItem;

//! Base class for all converters of SessionItem to/from TreeData object.

class MVVM_MODEL_EXPORT TreeDataItemConverterInterface
{
public:
  virtual ~TreeDataItemConverterInterface() = default;

  //! Creates TreeData from SessionItem.
  virtual std::unique_ptr<tree_data_t> ToTreeData(const SessionItem& item) const = 0;

  //! Creates SessionItem from TreeData.
  virtual std::unique_ptr<SessionItem> ToSessionItem(const tree_data_t&) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREEDATA_ITEM_CONVERTER_INTERFACE_H_
