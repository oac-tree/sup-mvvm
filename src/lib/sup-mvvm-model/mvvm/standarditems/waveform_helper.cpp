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

#include "waveform_helper.h"

#include <algorithm>

namespace mvvm
{

std::pair<std::vector<double>, std::vector<double> > GetPairOfVectors(const waveform_data_t& data)
{
  std::vector<double> first;
  std::vector<double> second;

  auto on_element = [&first, &second](const auto& element)
  {
    first.push_back(element.first);
    second.push_back(element.second);
  };
  std::for_each(std::begin(data), std::end(data), on_element);

  return {first, second};
}

}  // namespace mvvm
