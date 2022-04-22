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

#include "mvvm/model/itemcatalogue.h"

#include "mvvm/core/exceptions.h"
#include "mvvm/model/sessionitem.h"

#include <stdexcept>

namespace mvvm
{

//void ItemCatalogue::RegisterItem(const std::string& model_type, const item_factory_func_t& func,
//                                 const std::string& label)
//{
//  if (Contains(model_type))
//  {
//    throw ExistingKeyException("Attempt to add duplicate to item catalogue '" + model_type + "'");
//  }
//  m_info.push_back({model_type, label, func});
//}

//bool ItemCatalogue::Contains(const std::string& model_type) const
//{
//  auto it = find_if(m_info.begin(), m_info.end(),
//                    [model_type](auto element) { return element.item_type == model_type; });
//  return it != m_info.end();
//}

//std::unique_ptr<SessionItem> ItemCatalogue::Create(const std::string& model_type) const
//{
//  auto it = find_if(m_info.begin(), m_info.end(),
//                    [model_type](auto element) { return element.item_type == model_type; });
//  if (it == m_info.end())
//  {
//    throw NotFoundKeyException("No item registered for model type '" + model_type + "'");
//  }
//  return it->factory_func();
//}

//std::vector<std::string> ItemCatalogue::GetItemTypes() const
//{
//  std::vector<std::string> result;
//  for (const auto& x : m_info)
//  {
//    result.push_back(x.item_type);
//  }
//  return result;
//}

//std::vector<std::string> ItemCatalogue::GetLabels() const
//{
//  std::vector<std::string> result;
//  for (const auto& x : m_info)
//  {
//    result.push_back(x.item_label);
//  }
//  return result;
//}

//int ItemCatalogue::GetItemCount() const
//{
//  return static_cast<int>(m_info.size());
//}

////! Adds content of other catalogue to this.

//void ItemCatalogue::merge(const ItemCatalogue& other)
//{
//  for (const auto& it : other.m_info)
//  {
//    if (Contains(it.item_type))
//    {
//      throw std::runtime_error("ItemCatalogue::add() -> Catalogue contains duplicated records");
//    }

//    RegisterItem(it.item_type, it.factory_func, it.item_label);
//  }
//}

}  // namespace mvvm
