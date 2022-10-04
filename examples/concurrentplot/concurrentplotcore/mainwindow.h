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

#ifndef CONCURRENTPLOT_CONCURRENTPLOTCORE_MAINWINDOW_H
#define CONCURRENTPLOT_CONCURRENTPLOTCORE_MAINWINDOW_H

#include <QMainWindow>
#include <memory>

class GraphModel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent* event);

private:
  void initApplication();
  void writeSettings();
  void populateModel();

  std::unique_ptr<GraphModel> m_graph_model;
};

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_MAINWINDOW_H
