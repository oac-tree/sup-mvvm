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

#ifndef MVVM_MODEL_REPORTING_MODEL_V2_H_
#define MVVM_MODEL_REPORTING_MODEL_V2_H_

#include <mvvm/experimental/abstract_model_decorator_v2.h>

#include <functional>

namespace mvvm::experimental
{

class Item;

class ReportingModelV2 : public AbstractModelDecoratorV2
{
public:
  explicit ReportingModelV2(std::unique_ptr<ModelInterface> decorated_model);

  bool SetData(Item *item, const variant_t &data) override;

private:
  std::function<void(Item *)> m_on_data_change;
};

}  // namespace mvvm::experimental

#endif  // MVVM_MODEL_REPORTING_MODEL_V2_H_
