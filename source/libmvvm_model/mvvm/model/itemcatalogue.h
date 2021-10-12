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

#ifndef MVVM_MODEL_ITEMCATALOGUE_H
#define MVVM_MODEL_ITEMCATALOGUE_H

#include "mvvm/model/function_types.h"
#include "mvvm/model_export.h"

#include <string>
#include <vector>

namespace ModelView
{
class SessionItem;

//! Catalogue for item constructions. Contains collection of factory functions associated with
//! item's type and optional label.

class MVVM_MODEL_EXPORT ItemCatalogue
{
public:
  ItemCatalogue();
  ~ItemCatalogue();

  ItemCatalogue(const ItemCatalogue& other);
  ItemCatalogue& operator=(const ItemCatalogue& other);

  template <typename U>
  void RegisterItem(const std::string& label = {});

  void RegisterItem(const std::string& model_type, const item_factory_func_t& func,
                    const std::string& label = {});

  bool Contains(const std::string& model_type) const;

  std::unique_ptr<SessionItem> Create(const std::string& model_type) const;

  std::vector<std::string> GetItemTypes() const;

  std::vector<std::string> GetLabels() const;

  int GetItemCount() const;

  void merge(const ItemCatalogue& other);

private:
  struct ItemCatalogueImpl;
  std::unique_ptr<ItemCatalogueImpl> p_impl;
};

template <typename U>
void ItemCatalogue::RegisterItem(const std::string& label)
{
  RegisterItem(U().GetType(), ItemFactoryFunction<U>(), label);
}

}  // namespace ModelView

#endif  // MVVM_MODEL_ITEMCATALOGUE_H
