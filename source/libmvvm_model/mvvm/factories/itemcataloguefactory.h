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

#ifndef MVVM_FACTORIES_ITEMCATALOGUEFACTORY_H
#define MVVM_FACTORIES_ITEMCATALOGUEFACTORY_H

#include "mvvm/model/itemcatalogue.h"

namespace ModelView
{
//! Creates a catalog of items supported by SessionModel out-of-the-box.
MVVM_MODEL_EXPORT std::unique_ptr<ItemCatalogue> CreateStandardItemCatalogue();

//! Update user catalogue with items supported by SessionModel out-of-the-box.
MVVM_MODEL_EXPORT void AddStandardItemsToCatalogue(ItemCatalogue& user_catalogue);

}  // namespace ModelView

#endif  // MVVM_FACTORIES_ITEMCATALOGUEFACTORY_H
