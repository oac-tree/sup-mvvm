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

#include "custom_plot_test_utils.h"

#include <qcustomplot.h>

//! Returns vector representing bin centers/values on QCPGraph.

template <typename G, typename T>
std::vector<double> GetOperandValues(const G* graph, T operand)
{
  std::vector<double> result;
  auto graph_data = *graph->data();
  std::transform(std::begin(graph_data), std::end(graph_data), std::back_inserter(result),
                 [operand](const auto& point) { return operand(point); });
  return result;
}

std::vector<double> testutils::GetBinCenters(const QCPGraph* graph)
{
  return GetOperandValues(graph, [](auto x) { return x.key; });
}

std::vector<double> testutils::GetValues(const QCPGraph* graph)
{
  return GetOperandValues(graph, [](auto x) { return x.value; });
}

std::vector<double> testutils::GetErrors(const QCPGraph* graph)
{
  std::vector<double> result;
  if (auto errorBars = GetPlottable<QCPErrorBars>(graph->parentPlot()); errorBars)
  {
    auto container = errorBars->data();
    std::transform(container->begin(), container->end(), std::back_inserter(result),
                   [](auto x) { return x.errorPlus; });
  };
  return result;
}
