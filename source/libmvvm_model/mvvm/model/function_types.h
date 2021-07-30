// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_FUNCTION_TYPES_H
#define MVVM_MODEL_FUNCTION_TYPES_H

#include <functional>
#include <memory>

namespace ModelView
{
class SessionItem;

//! Definition for item factory funciton.
using item_factory_func_t = std::function<std::unique_ptr<SessionItem>()>;

//! Creates factory function for item of specific type.
template <typename T>
auto ItemFactoryFunction()
{
  return []() { return std::make_unique<T>(); };
}

}  // namespace ModelView

#endif
