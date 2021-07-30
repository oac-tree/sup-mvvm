// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/model/itemcatalogue.h"

#include "mvvm/model/sessionitem.h"

#include <stdexcept>

using namespace ModelView;

struct ItemCatalogue::ItemCatalogueImpl
{
  struct CatalogueEntry
  {
    std::string item_type;
    std::string item_label;
    item_factory_func_t factory_func;
  };

  std::vector<CatalogueEntry> m_info;
};

ItemCatalogue::ItemCatalogue() : p_impl(std::make_unique<ItemCatalogueImpl>()) {}

ItemCatalogue::~ItemCatalogue() = default;

ItemCatalogue::ItemCatalogue(const ItemCatalogue& other)
{
  p_impl = std::make_unique<ItemCatalogueImpl>(*other.p_impl);
}

ItemCatalogue& ItemCatalogue::operator=(const ItemCatalogue& other)
{
  if (this != &other)
  {
    ItemCatalogue tmp(other);
    std::swap(this->p_impl, tmp.p_impl);
  }
  return *this;
}

void ItemCatalogue::registerItem(const std::string& modelType, const item_factory_func_t& func,
                                 const std::string& label)
{
  if (contains(modelType))
    throw std::runtime_error("Error in ItemCatalogue: attempt to add dublicate '" + modelType
                             + "'");
  p_impl->m_info.push_back({modelType, label, func});
}

bool ItemCatalogue::contains(const std::string& modelType) const
{
  auto it = find_if(p_impl->m_info.begin(), p_impl->m_info.end(),
                    [modelType](auto element) { return element.item_type == modelType; });
  return it != p_impl->m_info.end();
}

std::unique_ptr<SessionItem> ItemCatalogue::create(const std::string& modelType) const
{
  auto it = find_if(p_impl->m_info.begin(), p_impl->m_info.end(),
                    [modelType](auto element) { return element.item_type == modelType; });
  if (it == p_impl->m_info.end())
    throw std::runtime_error("Error in ItemCatalogue: non existing '" + modelType + "'");
  return it->factory_func();
}

std::vector<std::string> ItemCatalogue::modelTypes() const
{
  std::vector<std::string> result;
  for (const auto& x : p_impl->m_info) result.push_back(x.item_type);
  return result;
}

std::vector<std::string> ItemCatalogue::labels() const
{
  std::vector<std::string> result;
  for (const auto& x : p_impl->m_info) result.push_back(x.item_label);
  return result;
}

int ItemCatalogue::itemCount() const
{
  return static_cast<int>(p_impl->m_info.size());
}

//! Adds content of other catalogue to this.

void ItemCatalogue::merge(const ItemCatalogue& other)
{
  for (const auto& it : other.p_impl->m_info)
  {
    if (contains(it.item_type))
      throw std::runtime_error("ItemCatalogue::add() -> Catalogue contains duplicated records");

    registerItem(it.item_type, it.factory_func, it.item_label);
  }
}
