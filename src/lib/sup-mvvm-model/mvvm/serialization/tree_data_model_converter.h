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

#ifndef MVVM_SERIALIZATION_TREE_DATA_MODEL_CONVERTER_H_
#define MVVM_SERIALIZATION_TREE_DATA_MODEL_CONVERTER_H_

#include <mvvm/serialization/converter_types.h>
#include <mvvm/serialization/i_tree_data_model_converter.h>

#include <memory>

namespace mvvm
{

//! Default converter of SessionModel to/from TreeData object.

class MVVM_MODEL_EXPORT TreeDataModelConverter : public ITreeDataModelConverter
{
public:
  explicit TreeDataModelConverter(ConverterMode converter_mode);
  ~TreeDataModelConverter() override;

  //! Returns true if given TreeData represents SessionModel object.
  bool IsSessionModelConvertible(const tree_data_t& tree_data) const;

  //! Creates TreeData from SessionModel.
  std::unique_ptr<tree_data_t> ToTreeData(const ISessionModel& model) const override;

  //! Populates empty SessionModel from TreeData.
  void PopulateSessionModel(const tree_data_t& tree_data, ISessionModel& model) const override;

private:
  struct TreeDataModelConverterImpl;
  std::unique_ptr<TreeDataModelConverterImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREE_DATA_MODEL_CONVERTER_H_
