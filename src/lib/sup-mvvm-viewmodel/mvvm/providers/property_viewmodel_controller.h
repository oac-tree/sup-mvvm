/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_PROVIDERS_PROPERTY_VIEWMODEL_CONTROLLER_H_
#define MVVM_PROVIDERS_PROPERTY_VIEWMODEL_CONTROLLER_H_

#include <mvvm/providers/viewmodel_controller.h>

namespace mvvm
{

class ISessionModel;
class ViewModelBase;

/**
 * @brief The PropertyViewModelController class is a controller intended for PropertyViewModel.
 *
 * It populates the view model using PropertyItemsStrategy to look for children, and
 * LabelDataRowStrategy to construct rows. It is meant to be extended to handle GroupItem in a
 * special way.
 */
class PropertyViewModelController : public ViewModelController
{
public:
  PropertyViewModelController(ISessionModel* model, ViewModelBase* view_model);
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_PROPERTY_VIEWMODEL_CONTROLLER_H_
