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

#include "mvvm/model/itemcatalogue.h"

#include "mvvm/model/sessionitem.h"

#include <stdexcept>

namespace ModelView
{
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

void ItemCatalogue::RegisterItem(const std::string& model_type, const item_factory_func_t& func,
                                 const std::string& label)
{
  if (Contains(model_type))
  {
    throw std::runtime_error("Error in ItemCatalogue: attempt to add dublicate '" + model_type
                             + "'");
  }
  p_impl->m_info.push_back({model_type, label, func});
}

bool ItemCatalogue::Contains(const std::string& model_type) const
{
  auto it = find_if(p_impl->m_info.begin(), p_impl->m_info.end(),
                    [model_type](auto element) { return element.item_type == model_type; });
  return it != p_impl->m_info.end();
}

std::unique_ptr<SessionItem> ItemCatalogue::Create(const std::string& model_type) const
{
  auto it = find_if(p_impl->m_info.begin(), p_impl->m_info.end(),
                    [model_type](auto element) { return element.item_type == model_type; });
  if (it == p_impl->m_info.end())
  {
    throw std::runtime_error("Error in ItemCatalogue: non existing '" + model_type + "'");
  }
  return it->factory_func();
}

std::vector<std::string> ItemCatalogue::GetItemTypes() const
{
  std::vector<std::string> result;
  for (const auto& x : p_impl->m_info)
  {
    result.push_back(x.item_type);
  }
  return result;
}

std::vector<std::string> ItemCatalogue::GetLabels() const
{
  std::vector<std::string> result;
  for (const auto& x : p_impl->m_info)
  {
    result.push_back(x.item_label);
  }
  return result;
}

int ItemCatalogue::GetItemCount() const
{
  return static_cast<int>(p_impl->m_info.size());
}

//! Adds content of other catalogue to this.

void ItemCatalogue::merge(const ItemCatalogue& other)
{
  for (const auto& it : other.p_impl->m_info)
  {
    if (Contains(it.item_type))
    {
      throw std::runtime_error("ItemCatalogue::add() -> Catalogue contains duplicated records");
    }

    RegisterItem(it.item_type, it.factory_func, it.item_label);
  }
}

}  // namespace ModelView
