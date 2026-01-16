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
}

void AppRestarter::StoreExitRequest(AppExitRequest exit_request)
{
  qApp->setProperty(kAppExitRequestProperty, static_cast<int>(exit_request));
}

std::int32_t AppRestarter::HandleExit(std::int32_t app_exit_code)
{
  auto stored_exit_code = qApp->property(kAppExitRequestProperty).toInt();

  HandleExitRequest(static_cast<mvvm::AppExitRequest>(stored_exit_code));

  // returning back same exit code as received from QApplication
  return app_exit_code;
}

void AppRestarter::HandleExitRequest(AppExitRequest exit_request)
{
  if (exit_request == AppExitRequest::CleanSettingsAndRestart)
  {
    // this will remove all settings for the current application (main window size)
    QSettings settings;
    settings.clear();

    // will start new main() in a separate process
    QProcess::startDetached(m_executable, m_args, m_working_path);
    return;
  }

  if (exit_request == AppExitRequest::Restart)
  {
    // will start new main() in a separate process
    QProcess::startDetached(m_executable, m_args, m_working_path);
    return;
  }

  // for NormalExit do nothing
}

}  // namespace mvvm
