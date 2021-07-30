// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_INTERFACES_ITEMFACTORYINTERFACE_H
#define MVVM_INTERFACES_ITEMFACTORYINTERFACE_H

#include "mvvm/model/function_types.h"

namespace ModelView
{
class SessionItem;

//! Interface class for all factories capable of producing SessionItem's.

class ItemFactoryInterface
{
public:
  virtual ~ItemFactoryInterface() = default;

  virtual void registerItem(const std::string& modelType, item_factory_func_t func,
                            const std::string& label) = 0;

  virtual std::unique_ptr<SessionItem> createItem(const std::string& modelType) const = 0;
};

}  // namespace ModelView

#endif  // MVVM_INTERFACES_ITEMFACTORYINTERFACE_H
