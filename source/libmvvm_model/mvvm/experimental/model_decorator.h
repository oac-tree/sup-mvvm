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

#ifndef MVVM_EXPERIMENTAL_MODEL_DECORATOR_H_
#define MVVM_EXPERIMENTAL_MODEL_DECORATOR_H_

#include <mvvm/experimental/abstract_model_decorator.h>
#include <mvvm/experimental/model.h>

namespace mvvm::experimental
{

template <typename T, typename... Types>
std::unique_ptr<ModelInterface> CreateModel()
{
  if constexpr (sizeof...(Types))
  {
    return std::make_unique<T>(CreateModel<Types...>());
  }
  else
  {
    return std::make_unique<T>();
  }
}

template <typename T, typename... Types>
class ModelDecorator : public AbstractModelDecorator
{
public:
  explicit ModelDecorator() : AbstractModelDecorator(CreateModel<T, Types...>()) {}

private:
};

}  // namespace mvvm::experimental

#endif  // MVVM_EXPERIMENTAL_MODEL_DECORATOR_H_
