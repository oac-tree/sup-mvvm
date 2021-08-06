// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/factories/itemcataloguefactory.h"

#include "mvvm/standarditems/standarditemincludes.h"

namespace ModelView
{

std::unique_ptr<ItemCatalogue> CreateStandardItemCatalogue()
{
  auto result = std::make_unique<ItemCatalogue>();
  result->RegisterItem<CompoundItem>();
  result->RegisterItem<PropertyItem>();
  result->RegisterItem<SessionItem>();
  result->RegisterItem<VectorItem>();
  return result;
}

void AddStandardItemsToCatalogue(ItemCatalogue &user_catalogue)
{
  user_catalogue.merge(*CreateStandardItemCatalogue());
}

}
