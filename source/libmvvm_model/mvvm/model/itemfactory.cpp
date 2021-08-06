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

void ItemFactory::RegisterItem(const std::string& model_type, item_factory_func_t func,
                               const std::string& label)
{
  m_catalogue->RegisterItem(model_type, func, label);
}

ItemFactory::~ItemFactory() = default;

std::unique_ptr<SessionItem> ItemFactory::CreateItem(const std::string& model_type) const
{
  return m_catalogue->Create(model_type);
}
