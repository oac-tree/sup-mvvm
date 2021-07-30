// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_ITEMFACTORY_H
#define MVVM_MODEL_ITEMFACTORY_H

#include "mvvm/interfaces/itemfactoryinterface.h"

#include <memory>

namespace ModelView
{
class ItemCatalogue;

//! Default SessionItem factory.

class ItemFactory : public ItemFactoryInterface
{
public:
  ItemFactory();
  ItemFactory(std::unique_ptr<ItemCatalogue> catalogue);
  ~ItemFactory() override;

  void registerItem(const std::string& modelType, item_factory_func_t func,
                    const std::string& label) override;

  std::unique_ptr<SessionItem> createItem(const std::string& modelType) const override;

protected:
  std::unique_ptr<ItemCatalogue> m_catalogue;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_ITEMFACTORY_H
