// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef CUSTOMPLOT_TEST_UTILS_H
#define CUSTOMPLOT_TEST_UTILS_H

#include <qcustomplot.h>

#include <algorithm>
#include <vector>

//! Various common utils for unit tests.

namespace TestUtils
{

//! Returns vector representing bin centers on QCPgraph.
std::vector<double> GetBinCenters(const QCPGraph* graph);

//! Returns vector representing y-values on QCPgraph.
std::vector<double> GetValues(const QCPGraph* graph);

//! Returns vector representing bin errors of QCPGraph.
std::vector<double> GetErrors(const QCPGraph* graph);

//! Finds and returns specific plottable in QCustomPlot canvas.
template <typename T>
T* GetPlottable(QCustomPlot* custom_plot)
{
  for (int i = 0; i < custom_plot->plottableCount(); ++i)
  {
    if (auto plottable = dynamic_cast<T*>(custom_plot->plottable()); plottable)
    {
      return plottable;
    }
  }
  return nullptr;
}
}  // namespace TestUtils

Q_DECLARE_METATYPE(QCPRange)

#endif
