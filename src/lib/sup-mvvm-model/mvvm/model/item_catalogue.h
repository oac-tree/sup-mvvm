/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_ITEM_CATALOGUE_H_
#define MVVM_MODEL_ITEM_CATALOGUE_H_

#include <mvvm/core/exceptions.h>
#include <mvvm/model_export.h>

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace mvvm
{
class SessionItem;

/**
 * @brief The ItemCatalogue class contains collection of factory functions associated with type
 * name and optional label.
 *
 * @tparam T The type to create with this catalogue.
 *
 * @details The idea is that catalogue can be used in simple global factory to register and produce
 * SessionItem of all possible types, or it can be used to register some smaller item groups with
 * narrow type. The label can be used in widgets instead of registered types, if necessary.
 *
 * @details
 */

template <typename T>
class MVVM_MODEL_EXPORT ItemCatalogue
{
public:
  using factory_func_t = std::function<std::unique_ptr<T>()>;

  /**
   * @brief Registers factory function for given type name.
   *
   * @param type_name The name of the type, normally coinciding with SessionItem::Type static
   * variable.
   * @param func Factory function to create item of this type.
   * @param label Optional label to use in widgets instead of type name.
   */
  void RegisterItem(const std::string& type_name, const factory_func_t& func,
                    const std::string& label = {});

  /**
   * @brief Registers factory function for given type.
   *
   * @param label Optional label to use in widgets instead of type name.
   */
  template <typename U>
  void RegisterItem(const std::string& label = {});

  /**
   * @brief Returns true if given type name is registered.
   */
  bool IsRegistered(const std::string& type_name) const;

  /**
   * @brief Create item with the given type name.
   */
  std::unique_ptr<T> Create(const std::string& type_name) const;

  /**
   * @brief Returns all registered item types.
   */
  std::vector<std::string> GetItemTypes() const;

  /**
   * @brief Returns all labels. Labels can be empty if user didn't provide them during the
   * registration.
   */
  std::vector<std::string> GetLabels() const;

  /**
   * @brief Returns total amount of registered types.
   */
  std::size_t GetItemCount() const;

  /**
   * @brief Adds content of other catalogue to this.
   */
  void Merge(const ItemCatalogue& other);

private:
  struct CatalogueEntry
  {
    std::string type_name;
    std::string item_label;
    factory_func_t factory_func;
  };

  std::vector<CatalogueEntry> m_info;
};

template <typename T>
void ItemCatalogue<T>::RegisterItem(const std::string& type_name, const factory_func_t& func,
                                    const std::string& label)
{
  if (IsRegistered(type_name))
  {
    throw ExistingKeyException("Attempt to add duplicate to item catalogue '" + type_name + "'");
  }
  m_info.push_back({type_name, label, func});
}

template <typename T>
template <typename U>
void ItemCatalogue<T>::RegisterItem(const std::string& label)
{
  RegisterItem(U::Type, []() { return std::make_unique<U>(); }, label);
}

template <typename T>
bool ItemCatalogue<T>::IsRegistered(const std::string& type_name) const
{
  auto iter = find_if(m_info.begin(), m_info.end(),
                      [type_name](auto element) { return element.type_name == type_name; });
  return iter != m_info.end();
}

template <typename T>
std::unique_ptr<T> ItemCatalogue<T>::Create(const std::string& type_name) const
{
  auto iter = find_if(m_info.begin(), m_info.end(),
                      [type_name](auto element) { return element.type_name == type_name; });
  if (iter == m_info.end())
  {
    throw KeyNotFoundException("No item registered for model type '" + type_name + "'");
  }
  return iter->factory_func();
}

template <typename T>
std::vector<std::string> ItemCatalogue<T>::GetItemTypes() const
{
  std::vector<std::string> result;
  for (const auto& info : m_info)
  {
    result.push_back(info.type_name);
  }
  return result;
}

template <typename T>
std::vector<std::string> ItemCatalogue<T>::GetLabels() const
{
  std::vector<std::string> result;
  for (const auto& info : m_info)
  {
    result.push_back(info.item_label);
  }
  return result;
}

template <typename T>
std::size_t ItemCatalogue<T>::GetItemCount() const
{
  return m_info.size();
}

template <typename T>
void ItemCatalogue<T>::Merge(const ItemCatalogue& other)
{
  for (const auto& iter : other.m_info)
  {
    if (IsRegistered(iter.type_name))
    {
      throw ExistingKeyException("Catalogue contains duplicated records");
    }

    RegisterItem(iter.type_name, iter.factory_func, iter.item_label);
  }
}

}  // namespace mvvm

#endif  // MVVM_MODEL_ITEM_CATALOGUE_H_
