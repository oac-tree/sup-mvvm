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

#ifndef MVVM_TREEDATAMODELCONVERTERINTERFACE_H
#define MVVM_TREEDATAMODELCONVERTERINTERFACE_H

#include "mvvm/model_export.h"

#include <memory>

namespace ModelView
{
class TreeData;
class SessionModel;

//! Base class for all converters of SessionModel to/from TreeData object.

class MVVM_MODEL_EXPORT TreeDataModelConverterInterface
{
public:
  virtual ~TreeDataModelConverterInterface() = default;

  //! Creates TreeData from SessionModel.
  virtual std::unique_ptr<TreeData> ToTreeData(const SessionModel& item) const = 0;

  //! Populates empty SessionModel from TreeData.
  virtual void PopulateSessionModel(const TreeData&, SessionModel&) const = 0;
};

}  // namespace ModelView

#endif  // MVVM_TREEDATAMODELCONVERTERINTERFACE_H
