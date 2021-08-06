// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

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
//! item's modelType and optional label.

class MVVM_MODEL_EXPORT ItemCatalogue
{
public:
  ItemCatalogue();
  ~ItemCatalogue();

  ItemCatalogue(const ItemCatalogue& other);
  ItemCatalogue& operator=(const ItemCatalogue& other);

  template <typename T>
  void RegisterItem(const std::string& label = {});

  void RegisterItem(const std::string& model_type, const item_factory_func_t& func,
                    const std::string& label);

  bool Contains(const std::string& model_type) const;

  std::unique_ptr<SessionItem> Create(const std::string& model_type) const;

  std::vector<std::string> GetModelTypes() const;

  std::vector<std::string> GetLabels() const;

  int GetItemCount() const;

  void merge(const ItemCatalogue& other);

private:
  struct ItemCatalogueImpl;
  std::unique_ptr<ItemCatalogueImpl> p_impl;
};

template <typename T>
void ItemCatalogue::RegisterItem(const std::string& label)
{
  RegisterItem(T::Type, ItemFactoryFunction<T>(), label);
}

}  // namespace ModelView

#endif  // MVVM_MODEL_ITEMCATALOGUE_H
