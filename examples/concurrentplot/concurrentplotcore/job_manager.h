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

#ifndef CONCURRENTPLOT_CONCURRENTPLOTCORE_JOBMANAGER_H
#define CONCURRENTPLOT_CONCURRENTPLOTCORE_JOBMANAGER_H

#include "toy_simulation.h"

#include <mvvm/utils/threadsafe_stack.h>

#include <QObject>

namespace concurrentplot
{

//! Handles all thread activity for running job simulation in the background.

class JobManager : public QObject
{
  Q_OBJECT

public:
  JobManager(QObject* parent = nullptr);
  ~JobManager() override;

  std::vector<double> GetSimulationResult();

signals:
  void progressChanged(int value);
  void simulationCompleted();

public slots:
  void RequestSimulation(double value);
  void SetDelay(int value);
  void OnInterruptRequest();

private:
  void WaitAndRun();

  std::thread m_sim_thread;
  mvvm::threadsafe_stack<double> m_requested_values;
  mvvm::threadsafe_stack<ToySimulation::Result> m_simulation_results;
  std::atomic<bool> m_is_running;
  bool m_interrupt_request{false};
  int m_delay{0};
};

}  // namespace concurrentplot

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_JOBMANAGER_H
