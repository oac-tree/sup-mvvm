// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/model/itemfactory.h"

#include "mvvm/model/itemcatalogue.h"
#include "mvvm/model/sessionitem.h"

using namespace ModelView;

ItemFactory::ItemFactory() : m_catalogue(std::make_unique<ItemCatalogue>()) {}

ItemFactory::ItemFactory(std::unique_ptr<ItemCatalogue> catalogue)
    : m_catalogue(std::move(catalogue))
{
}

void ItemFactory::registerItem(const std::string& modelType, item_factory_func_t func,
                               const std::string& label)
{
  m_catalogue->registerItem(modelType, func, label);
}

ItemFactory::~ItemFactory() = default;

std::unique_ptr<SessionItem> ItemFactory::createItem(const std::string& modelType) const
{
  return m_catalogue->create(modelType);
}
