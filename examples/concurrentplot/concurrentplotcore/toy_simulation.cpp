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

namespace
{
constexpr double pi = 3.14159265358979323846;
}

namespace concurrentplot
{

//! Run of heavy toy computational task.

ToySimulation::ToySimulation(double amplitude, int delay) : m_delay(delay)
{
  m_progress_handler.SetMaxTicksCount(m_input_data.npoints);
  m_input_data.amplitude = amplitude;
}

void ToySimulation::RunSimulation()
{
  m_progress_handler.Reset();
  m_result.data.clear();
  m_result.xmin = m_input_data.xmin;
  m_result.xmax = m_input_data.xmax;

  const double dx = (m_input_data.xmax - m_input_data.xmin) / m_input_data.npoints;
  for (int i = 0; i < m_input_data.npoints; ++i)
  {
    if (m_progress_handler.HasInterruptRequest())
    {
      throw std::runtime_error("Interrupt request");
    }
    double x = m_input_data.xmin + i * dx;
    double value = m_input_data.amplitude * 10.0 * std::sin(2.0 * pi * 2 * x)
                   + m_input_data.amplitude * 5.0 * std::sin(2 * pi * 2.25 * x);
    m_result.data.push_back(value);

    m_progress_handler.SetCompletedTicks(1);

    if (m_delay > 0)
    {
      std::this_thread::sleep_for(std::chrono::microseconds(m_delay));
    }
  }
}

//! Returns result of toy simulation.

ToySimulation::Result ToySimulation::GetSimulationResult() const
{
  return m_result;
}

void ToySimulation::SetProgressCallback(mvvm::ProgressHandler::callback_t callback)
{
  m_progress_handler.Subscribe(std::move(callback));
}

}  // namespace concurrentplot
