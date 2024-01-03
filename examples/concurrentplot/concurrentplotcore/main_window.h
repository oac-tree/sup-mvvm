/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef CONCURRENTPLOT_CONCURRENTPLOTCORE_MAIN_WINDOW_H_
#define CONCURRENTPLOT_CONCURRENTPLOTCORE_MAIN_WINDOW_H_

#include <QMainWindow>
#include <memory>

namespace concurrentplot
{

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
  void InitApplication();
  void WriteSettings();
  void PopulateModel();

  std::unique_ptr<GraphModel> m_graph_model;
};

}  // namespace concurrentplot

#endif  // CONCURRENTPLOT_CONCURRENTPLOTCORE_MAIN_WINDOW_H_
