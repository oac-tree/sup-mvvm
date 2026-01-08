/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef APPLAUNCHER_MAIN_WINDOW_H_
#define APPLAUNCHER_MAIN_WINDOW_H_

#include <QMainWindow>

namespace applauncher
{

class CentralWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow() override;

protected:
  void closeEvent(QCloseEvent* event) override;

private:
  void InitApplication();
  void WriteSettings();
  void OnExitRequest(int exit_code);

  CentralWidget* m_central_widget{nullptr};
};

}  // namespace applauncher

#endif  // APPLAUNCHER_MAIN_WINDOW_H_
