/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Authors       : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_TREEDATAITEMCONVERTER_H
#define MVVM_TREEDATAITEMCONVERTER_H

#include "mvvm/serialization/treedataitemconverterinterface.h"

namespace ModelView
{
class ItemFactoryInterface;

//! Converters SessionItem to/from TreeData object.

class MVVM_MODEL_EXPORT TreeDataItemConverter : public TreeDataItemConverterInterface
{
public:
  TreeDataItemConverter(const ItemFactoryInterface* factory);
  ~TreeDataItemConverter();

  //! Returns true if given TreeData represents SessionItem object.
  bool IsSessionItemConvertible(const TreeData& tree_data) const;

  //! Creates SessionItem from TreeData.
  std::unique_ptr<SessionItem> ToSessionItem(const TreeData&) const override;

  //! Creates TreeData from SessionItem.
  std::unique_ptr<TreeData> ToTreeData(const SessionItem& item) const override;

private:
  struct TreeDataItemConverterImpl;
  std::unique_ptr<TreeDataItemConverterImpl> p_impl;
};

}  // namespace ModelView

#endif  // MVVM_TREEDATAITEMCONVERTER_H
