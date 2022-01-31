// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "customplot_test_utils.h"

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

std::vector<double> TestUtils::GetBinCenters(const QCPGraph* graph)
{
  return GetOperandValues(graph, [](auto x) { return x.key; });
}

std::vector<double> TestUtils::GetValues(const QCPGraph* graph)
{
  return GetOperandValues(graph, [](auto x) { return x.value; });
}

std::vector<double> TestUtils::GetErrors(const QCPGraph* graph)
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
