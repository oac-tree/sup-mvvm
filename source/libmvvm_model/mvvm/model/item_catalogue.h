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

#ifndef MVVM_MODEL_ITEMCATALOGUE_H
#define MVVM_MODEL_ITEMCATALOGUE_H

#include "mvvm/core/exceptions.h"
#include "mvvm/model_export.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace mvvm
{
class SessionItem;

//! Catalogue for item constructions. Contains collection of factory functions associated with
//! item's type and optional label.

template <typename T>
class MVVM_MODEL_EXPORT ItemCatalogue
{
public:
  using factory_func_t = std::function<std::unique_ptr<T>()>;

  template <typename U>
  void RegisterItem(const std::string& label = {});

  void RegisterItem(const std::string& item_type, const factory_func_t& func,
                    const std::string& label = {});

  bool Contains(const std::string& item_type) const;

  std::unique_ptr<T> Create(const std::string& item_type) const;

  std::vector<std::string> GetItemTypes() const;

  std::vector<std::string> GetLabels() const;

  int GetItemCount() const;

  void Merge(const ItemCatalogue& other);

private:
  struct CatalogueEntry
  {
    std::string item_type;
    std::string item_label;
    factory_func_t factory_func;
  };

  std::vector<CatalogueEntry> m_info;
};

template <typename T>
template <typename U>
void ItemCatalogue<T>::RegisterItem(const std::string& label)
{
  RegisterItem(
      U().GetType(), []() { return std::make_unique<U>(); }, label);
}

template <typename T>
void ItemCatalogue<T>::RegisterItem(const std::string& item_type, const factory_func_t& func,
                                    const std::string& label)
{
  if (Contains(item_type))
  {
    throw ExistingKeyException("Attempt to add duplicate to item catalogue '" + item_type + "'");
  }
  m_info.push_back({item_type, label, func});
}

template <typename T>
bool ItemCatalogue<T>::Contains(const std::string& item_type) const
{
  auto it = find_if(m_info.begin(), m_info.end(),
                    [item_type](auto element) { return element.item_type == item_type; });
  return it != m_info.end();
}

template <typename T>
std::unique_ptr<T> ItemCatalogue<T>::Create(const std::string& item_type) const
{
  auto it = find_if(m_info.begin(), m_info.end(),
                    [item_type](auto element) { return element.item_type == item_type; });
  if (it == m_info.end())
  {
    throw NotFoundKeyException("No item registered for model type '" + item_type + "'");
  }
  return it->factory_func();
}

template <typename T>
std::vector<std::string> ItemCatalogue<T>::GetItemTypes() const
{
  std::vector<std::string> result;
  for (const auto& x : m_info)
  {
    result.push_back(x.item_type);
  }
  return result;
}

template <typename T>
std::vector<std::string> ItemCatalogue<T>::GetLabels() const
{
  std::vector<std::string> result;
  for (const auto& x : m_info)
  {
    result.push_back(x.item_label);
  }
  return result;
}

template <typename T>
int ItemCatalogue<T>::GetItemCount() const
{
  return static_cast<int>(m_info.size());
}

//! Adds content of other catalogue to this.

template <typename T>
void ItemCatalogue<T>::Merge(const ItemCatalogue& other)
{
  for (const auto& it : other.m_info)
  {
    if (Contains(it.item_type))
    {
      throw ExistingKeyException("Catalogue contains duplicated records");
    }

    RegisterItem(it.item_type, it.factory_func, it.item_label);
  }
}

}  // namespace mvvm

#endif  // MVVM_MODEL_ITEMCATALOGUE_H
