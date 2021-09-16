/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
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

#ifndef MVVM_VIEWMODEL_PRESENTATIONITEMFACTORY_H
#define MVVM_VIEWMODEL_PRESENTATIONITEMFACTORY_H

//! @file presentationitemfactory.h
//! Contains factory methods for various presentations of different SessionItem parts.

#include "mvvm/viewmodelbase/viewitemdatainterface.h"

#include <memory>

namespace ModelView
{
class SessionItem;
class ModelComposer;

//! Creates read-only presentation of the SessionItem's display name.
std::unique_ptr<ViewItemDataInterface> CreateDisplayNamePresentation(SessionItem* item);

//! Creates editable presentation of the SessionItem's display name.
std::unique_ptr<ViewItemDataInterface> CreateEditableDisplayNamePresentation(SessionItem* item);

//! Creates presentation of the SessionItem's data.
std::unique_ptr<ViewItemDataInterface> CreateDataPresentation(SessionItem* item,
                                                              ModelComposer* composer = nullptr);

}  // namespace ModelView

#endif  // MVVM_VIEWMODEL_PRESENTATIONITEMFACTORY_H
