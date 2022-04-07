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

#ifndef MVVM_SERIALIZATION_TREEDATAMODELCONVERTER_H
#define MVVM_SERIALIZATION_TREEDATAMODELCONVERTER_H

#include "mvvm/serialization/converter_types.h"
#include "mvvm/serialization/treedatamodelconverterinterface.h"

#include <memory>

namespace mvvm
{
class TreeData;
class SessionModel;

//! Default converter of SessionModel to/from TreeData object.

class MVVM_MODEL_EXPORT TreeDataModelConverter : public TreeDataModelConverterInterface
{
public:
  TreeDataModelConverter(ConverterMode converter_mode);
  ~TreeDataModelConverter();

  //! Returns true if given TreeData represents SessionModel object.
  bool IsSessionModelConvertible(const TreeData& tree_data) const;

  //! Creates TreeData from SessionModel.
  std::unique_ptr<TreeData> ToTreeData(const SessionModel& model) const override;

  //! Populates empty SessionModel from TreeData.
  void PopulateSessionModel(const TreeData& tree_data, SessionModel& model) const override;

private:
  struct TreeDataModelConverterImpl;
  std::unique_ptr<TreeDataModelConverterImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREEDATAMODELCONVERTER_H
