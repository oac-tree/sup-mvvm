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

#ifndef MVVM_MODEL_MODEL_DECORATOR_H_
#define MVVM_MODEL_MODEL_DECORATOR_H_

#include <mvvm/model/abstract_model_decorator.h>

namespace mvvm
{


//! Create model decorators.
//!
//! auto model = CreateModel<ReportingModel, Model>();
//! is equivalent of
//! auto model = std::make_unique_ptr<ReportingModel>(std::make_unique_ptr<Model>);

template <typename T, typename... Types, typename... Args>
std::unique_ptr<SessionModelInterface> CreateModel(Args &&...args)
{
  if constexpr (sizeof...(Types))
  {
    return std::make_unique<T>(CreateModel<Types...>(std::forward<Args>(args)...));
  }
  else
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }
}

//! Universal model decorator.
//!
//! ModelDecorator<ReportingModel, Model> model;
//! will build AbstractModelDecorator based class with the model being decorated
//! unique_ptr<ReportingModel>(std::make_unique_ptr<Model>)

template <typename T, typename... Types>
class ModelDecorator : public AbstractModelDecorator
{
public:
  explicit ModelDecorator() : AbstractModelDecorator(CreateModel<T, Types...>()) {}

private:
};

}  // namespace mvvm

#endif  // MVVM_MODEL_MODEL_DECORATOR_H_
