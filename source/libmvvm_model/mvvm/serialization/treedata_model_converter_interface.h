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

#ifndef MVVM_SERIALIZATION_TREEDATA_MODEL_CONVERTER_INTERFACE_H_
#define MVVM_SERIALIZATION_TREEDATA_MODEL_CONVERTER_INTERFACE_H_

#include <mvvm/model_export.h>

#include <memory>

namespace mvvm
{
class TreeData;
class SessionModelInterface;

//! Base class for all converters of SessionModel to/from TreeData object.

class MVVM_MODEL_EXPORT TreeDataModelConverterInterface
{
public:
  virtual ~TreeDataModelConverterInterface() = default;

  //! Creates TreeData from SessionModel.
  virtual std::unique_ptr<TreeData> ToTreeData(const SessionModelInterface& item) const = 0;

  //! Populates empty SessionModel from TreeData.
  virtual void PopulateSessionModel(const TreeData&, SessionModelInterface&) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREEDATA_MODEL_CONVERTER_INTERFACE_H_
