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

#include "mvvm/experimental/reporting_model_v2.h"

#include "mvvm/experimental/model.h"

namespace mvvm::experimental
{

ReportingModelV2::ReportingModelV2(std::unique_ptr<ModelInterface> decorated_model)
    : AbstractModelDecorator(std::move(decorated_model))
{
}

bool ReportingModelV2::SetData(Item *item, const variant_t &data)
{
  auto result = AbstractModelDecorator::SetData(item, data);
  if (result && m_on_data_change)
  {
    m_on_data_change(item);
  }

  return result;
}

}  // namespace mvvm::experimental
