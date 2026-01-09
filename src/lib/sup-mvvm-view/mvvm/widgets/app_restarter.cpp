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

#include "app_restarter.h"

#include <QApplication>
#include <QDir>
#include <QProcess>
#include <QSettings>

namespace mvvm
{

AppRestarter::AppRestarter(int argc, char* argv[])
    : m_executable(QString::fromLocal8Bit(argv[0])), m_working_path(QDir::currentPath())
{
  Q_UNUSED(argc)
  qDebug() << "AppRestarter" << m_executable << m_working_path;
}

int32_t AppRestarter::HandleExitCode(int32_t app_exit_code)
{
  qDebug() << "AppRestarter::HandleExitCode -> app_exit_code" << app_exit_code;

  auto stored_exit_code = qApp->property(mvvm::constants::kAppExitCodeProperty).toInt();

  HandleAppExitCode(static_cast<mvvm::AppExitCode>(stored_exit_code));

  return app_exit_code;  // returning back same exit code as received from QApplication
}

void AppRestarter::HandleAppExitCode(AppExitCode stored_exit_code)
{
  qDebug() << "AppRestarter::HandleAppExitCode -> stored_exit_code"
           << static_cast<int>(stored_exit_code);

  if (stored_exit_code == AppExitCode::CleanSettingsAndRestart)
  {
    // this will remove all settings for the current application (main window size)
    QSettings settings;
    settings.clear();
    qDebug() << "AppRestarter::HandleAppExitCode -> startDetached Restart 1.1";
    QProcess::startDetached(m_executable, m_args, m_working_path);
    qDebug() << "AppRestarter::HandleAppExitCode -> startDetached Restart 1.2";
  }

  if (stored_exit_code == AppExitCode::Restart)
  {
    qDebug() << "AppRestarter::HandleAppExitCode -> startDetached Restart 1.1";
    QProcess::startDetached(m_executable, m_args, m_working_path);
    qDebug() << "AppRestarter::HandleAppExitCode -> startDetached Restart 1.2";
  }
}

}  // namespace mvvm
