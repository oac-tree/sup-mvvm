/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef TESTS_TESTMVVM_INTEGRATION_CUSTOM_PLOT_TEST_UTILS_H_
#define TESTS_TESTMVVM_INTEGRATION_CUSTOM_PLOT_TEST_UTILS_H_

#include <qcustomplot.h>

#include <algorithm>
#include <vector>

//! Various common utils for unit tests.

namespace testutils
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
}  // namespace testutils

Q_DECLARE_METATYPE(QCPRange)

#endif  // TESTS_TESTMVVM_INTEGRATION_CUSTOM_PLOT_TEST_UTILS_H_
