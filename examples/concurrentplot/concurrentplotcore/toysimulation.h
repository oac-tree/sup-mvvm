// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef CONCURRENTPLOTCORE_TOYSIMULATION_H
#define CONCURRENTPLOTCORE_TOYSIMULATION_H

#include "mvvm/utils/progress_handler.h"

#include <vector>

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

  ToySimulation(double amplitude = 1.0, int delay = 0.0);

  void runSimulation();

  Result simulationResult() const;

  void setProgressCallback(mvvm::ProgressHandler::callback_t callback);

private:
  InputData input_data;
  Result result;
  int delay{0};
  mvvm::ProgressHandler progress_handler;
};

#endif  // CONCURRENTPLOTCORE_TOYSIMULATION_H
