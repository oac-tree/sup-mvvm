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

#ifndef MVVM_WIDGETS_APP_RESTARTER_H_
#define MVVM_WIDGETS_APP_RESTARTER_H_

#include <mvvm/widgets/main_window_types.h>

#include <QString>
#include <QStringList>
#include <cstdint>

namespace mvvm
{

/**
 * @brief The AppRestarter class handles restart of the application.
 *
 * Depending on exit code, it can restart application in a separate process, clear settings, or let
 * it exit.
 */
class AppRestarter
{
public:
  AppRestarter(int argc, char* argv[]);

  /**
   * @brief Handles exit code of the application.
   *
   * @param app_exit_code The exit code as reported by QApplication::exec() method
   * @return Returns the final exit code to be returned by main() function.
   */
  std::int32_t HandleExitCode(std::int32_t app_exit_code);

private:
  void HandleAppExitCode(mvvm::AppExitCode stored_exit_code);

  QString m_executable;
  QString m_working_path;
  QStringList m_args;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_APP_RESTARTER_H_
