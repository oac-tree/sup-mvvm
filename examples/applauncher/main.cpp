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

#include "main_window.h"

#include <mvvm/widgets/app_restarter.h>

#include <QApplication>

int main(int argc, char** argv)
{
  mvvm::AppRestarter restarter(argc, argv);

  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

  QApplication app(argc, argv);

  applauncher::MainWindow win;
  win.show();

  return restarter.HandleExit(app.exec());
}
