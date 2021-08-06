// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

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
