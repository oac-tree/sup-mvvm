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

class MVVM_MODEL_EXPORT ItemFactory : public ItemFactoryInterface
{
public:
  ItemFactory();
  ItemFactory(std::unique_ptr<ItemCatalogue> catalogue);
  ~ItemFactory() override;

  void RegisterItem(const std::string& model_type, item_factory_func_t func,
                    const std::string& label) override;

  std::unique_ptr<SessionItem> CreateItem(const std::string& model_type) const override;

protected:
  std::unique_ptr<ItemCatalogue> m_catalogue;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_ITEMFACTORY_H
