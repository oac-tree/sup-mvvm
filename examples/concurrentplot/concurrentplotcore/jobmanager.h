// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef CONCURRENTPLOTCORE_JOBMANAGER_H
#define CONCURRENTPLOTCORE_JOBMANAGER_H

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

#endif  // CONCURRENTPLOTCORE_JOBMANAGER_H
