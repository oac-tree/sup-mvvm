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

#include "toy_simulation.h"

#include <chrono>
#include <cmath>
#include <stdexcept>
#include <thread>

using namespace mvvm;

namespace
{
constexpr double pi = 3.14159265358979323846;
}

//! Run of heavy toy computational task.

ToySimulation::ToySimulation(double amplitude, int delay) : delay(delay)
{
  progress_handler.setMaxTicksCount(input_data.npoints);
  input_data.amplitude = amplitude;
}

void ToySimulation::runSimulation()
{
  progress_handler.reset();
  result.data.clear();
  result.xmin = input_data.xmin;
  result.xmax = input_data.xmax;

  const double dx = (input_data.xmax - input_data.xmin) / input_data.npoints;
  for (int i = 0; i < input_data.npoints; ++i)
  {
    if (progress_handler.has_interrupt_request())
      throw std::runtime_error("Interrupt request");
    double x = input_data.xmin + i * dx;
    double value = input_data.amplitude * 10.0 * std::sin(2.0 * pi * 2 * x)
                   + input_data.amplitude * 5.0 * std::sin(2 * pi * 2.25 * x);
    result.data.push_back(value);

    progress_handler.setCompletedTicks(1);

    if (delay > 0)
      std::this_thread::sleep_for(std::chrono::microseconds(delay));
  }
}

//! Returns result of toy simulation.

ToySimulation::Result ToySimulation::simulationResult() const
{
  return result;
}

void ToySimulation::setProgressCallback(ProgressHandler::callback_t callback)
{
  progress_handler.subscribe(std::move(callback));
}
