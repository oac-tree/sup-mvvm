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

#ifndef MVVM_MODEL_REPORTING_MODEL_H_
#define MVVM_MODEL_REPORTING_MODEL_H_

#include <mvvm/core/variant.h>
#include <mvvm/experimental/model_interface.h>

#include <functional>

namespace mvvm::experimental
{

class Item;

template <typename T>
class ReportingModel : public T
{
public:
  static_assert(std::is_base_of<ModelInterface, T>::value, "Invalid template argument");

  template <typename... Args>
  explicit ReportingModel(Args &&...args) : T(std::forward<Args>(args)...)
  {
  }

  bool SetData(Item *item, const variant_t &data);

private:
  std::function<void(Item *)> m_on_data_change;
};

template <typename T>
bool ReportingModel<T>::SetData(Item *item, const variant_t &data)
{
  auto result = T::SetData(item, data);
  if (result && m_on_data_change)
  {
    m_on_data_change(item);
  }

  return result;
}

}  // namespace mvvm::experimental

#endif  // MVVM_MODEL_REPORTING_MODEL_H_
