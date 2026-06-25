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

#include "property_viewmodel.h"

#include <mvvm/providers/property_viewmodel_controller.h>

#include <memory>

namespace mvvm
{
PropertyViewModel::PropertyViewModel(ISessionModel* model, QObject* parent_object)
    : ViewModel(parent_object)
{
  SetController(std::make_unique<PropertyViewModelController>(model, this));
}

}  // namespace mvvm
