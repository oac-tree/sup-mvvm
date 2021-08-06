// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_INTERFACES_ITEMFACTORYINTERFACE_H
#define MVVM_INTERFACES_ITEMFACTORYINTERFACE_H

#include "mvvm/model/function_types.h"
#include "mvvm/model_export.h"

namespace ModelView
{
class SessionItem;

//! Interface class for all factories capable of producing SessionItem's.

class MVVM_MODEL_EXPORT ItemFactoryInterface
{
public:
  virtual ~ItemFactoryInterface() = default;

  virtual void RegisterItem(const std::string& model_type, item_factory_func_t func,
                            const std::string& label) = 0;

  virtual std::unique_ptr<SessionItem> CreateItem(const std::string& model_type) const = 0;
};

}  // namespace ModelView

#endif  // MVVM_INTERFACES_ITEMFACTORYINTERFACE_H
