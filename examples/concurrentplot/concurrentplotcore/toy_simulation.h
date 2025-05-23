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

#ifndef CONCURRENTPLOT_CONCURRENTPLOTCORE_TOY_SIMULATION_H_
#define CONCURRENTPLOT_CONCURRENTPLOTCORE_TOY_SIMULATION_H_

#include <mvvm/utils/progress_handler.h>

#include <vector>

namespace concurrentplot
{

//! Represents long running toy scientific simulation.

class ToySimulation
{
public:
  //! Represents input conditions for toy simulation.
  struct InputData
  {
    double xmin{0.0};
    double xmax{5.0};
    int npoints{400};
    double amplitude{1.0};
  };

  //! Represents results of the simulation.
  struct Result
  {
    double xmin{0.0};
    double xmax{5.0};
    std::vector<double> data;
  };

  explicit ToySimulation(double amplitude = 1.0, int delay = 0.0);

  void RunSimulation();

  Result GetSimulationResult() const;

  void SetProgressCallback(mvvm::ProgressHandler::callback_t callback);

private:
  InputData m_input_data;
  Result m_result;
  int m_delay{0};
  mvvm::ProgressHandler m_progress_handler;
};

}  // namespace concurrentplot

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_TOY_SIMULATION_H_
