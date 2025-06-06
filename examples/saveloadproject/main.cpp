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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "saveloadprojectcore/main_window.h"

#include <QApplication>
#include <QLocale>

int main(int argc, char** argv)
{
  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
  QCoreApplication::setApplicationName("saveloadproject");
  QCoreApplication::setApplicationVersion("0.1");
  QCoreApplication::setOrganizationName("sup-mvvm");

  QApplication app(argc, argv);

  saveloadproject::MainWindow win;
  win.show();

  return app.exec();
}
