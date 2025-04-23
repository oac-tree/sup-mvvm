/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "waveform_helper.h"

namespace mvvm
{

std::pair<std::vector<double>, std::vector<double> > GetPairOfVectors(const waveform_data_t& data)
{
  std::vector<double> first;
  std::vector<double> second;

  for (const auto& element : data)
  {
    first.push_back(element.first);
    second.push_back(element.second);
  }

  return {first, second};
}

}  // namespace mvvm
