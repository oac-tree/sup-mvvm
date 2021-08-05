// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/factories/itemcataloguefactory.h"

#include "mvvm/standarditems/standarditemincludes.h"

using namespace ModelView;

std::unique_ptr<ItemCatalogue> ModelView::CreateStandardItemCatalogue()
{
  auto result = std::make_unique<ItemCatalogue>();
  result->registerItem<CompoundItem>();
  result->registerItem<PropertyItem>();
  result->registerItem<SessionItem>();
  result->registerItem<VectorItem>();
  return result;
}
