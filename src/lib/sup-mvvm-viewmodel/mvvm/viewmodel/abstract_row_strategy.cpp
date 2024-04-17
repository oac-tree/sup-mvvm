/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "abstract_row_strategy.h"

#include <mvvm/viewmodelbase/viewitem.h>
#include <mvvm/core/exceptions.h>

namespace mvvm
{

int AbstractRowStrategy::GetSize() const
{
  return GetHorizontalHeaderLabels().size();
}

std::vector<std::unique_ptr<ViewItem> > AbstractRowStrategy::ConstructRow(SessionItem *item)
{
  if (!item)
  {
    return {};
  }

  auto result = ConstructRowImpl(item);
  if (result.size() != GetSize())
  {
    throw RuntimeException("Size of row doesn't match");
  }

  return ConstructRowImpl(item);
}

}  // namespace mvvm
