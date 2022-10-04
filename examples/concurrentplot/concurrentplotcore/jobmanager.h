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

#include "mvvm/utils/threadsafe_stack.h"
#include "toysimulation.h"

#include <QObject>

//! Handles all thread activity for running job simulation in the background.

class JobManager : public QObject
{
  Q_OBJECT

public:
  JobManager(QObject* parent = nullptr);
  ~JobManager() override;

  std::vector<double> simulationResult();

signals:
  void progressChanged(int value);
  void simulationCompleted();

public slots:
  void requestSimulation(double value);
  void setDelay(int value);
  void onInterruptRequest();

private:
  void wait_and_run();

  std::thread sim_thread;
  mvvm::threadsafe_stack<double> requested_values;
  mvvm::threadsafe_stack<ToySimulation::Result> simulation_results;
  std::atomic<bool> is_running;
  bool interrupt_request{false};
  int delay{0};
};

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_JOBMANAGER_H
