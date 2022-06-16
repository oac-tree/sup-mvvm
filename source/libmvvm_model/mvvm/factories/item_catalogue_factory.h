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

#ifndef MVVM_FACTORIES_ITEM_CATALOGUE_FACTORY_H_
#define MVVM_FACTORIES_ITEM_CATALOGUE_FACTORY_H_

#include <mvvm/model/item_catalogue.h>

namespace mvvm
{
//! Creates a catalog of items supported by SessionModel out-of-the-box.
MVVM_MODEL_EXPORT std::unique_ptr<ItemCatalogue<SessionItem>> CreateStandardItemCatalogue();

//! Update user catalogue with items supported by SessionModel out-of-the-box.
MVVM_MODEL_EXPORT void AddStandardItemsToCatalogue(ItemCatalogue<SessionItem>& user_catalogue);

}  // namespace mvvm

#endif  // MVVM_FACTORIES_ITEM_CATALOGUE_FACTORY_H_
