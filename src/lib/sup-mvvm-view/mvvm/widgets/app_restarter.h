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
 * @brief The AppRestarter class handles the restart of the application.
 *
 * It is expected that restart request is stored in custom property of QApplication. Depending on
 * its value, it can restart the application in a separate process, clear settings, or just exit.
 *
 * In the code below, we distinguish "exit code" - the value returned by QApplication::exec()
 * method, and "exit request" - the value stored by the MainWindow in custom property of
 * QApplication. The latter indicates what should be done on application exit.
 */
class AppRestarter
{
public:
  //! the name of the QApplication custom property to store exit request
  static constexpr auto kAppExitRequestProperty = "app_exit_request";

  AppRestarter(int argc, char* argv[]);

  /**
   * @brief Handles what will happen on application exit.
   *
   * @param app_exit_code The exit code as reported by QApplication::exec() method.
   * @return Returns the final exit code to be returned by main() function.
   *
   * @details This method checks application exit request stored in the QApplication property
   * and proceed accordingly: restart application, clear settings and restart, or just exit.
   * It returns back the same exit code as received from QApplication.
   */
  std::int32_t HandleExit(std::int32_t app_exit_code);

  /**
   * @brief Stores application exit request in the QApplication property.
   */
  static void StoreExitRequest(AppExitRequest exit_request);

private:
  /**
   * @brief Handles application exit request.
   *
   * @param exit_request The exit request type.
   *
   * @details Depending on the exit request type, this method can restart application in a
   * separate process, clear settings and restart, or just exit.
   */
  void HandleExitRequest(mvvm::AppExitRequest exit_request);

  QString m_executable;
  QString m_working_path;
  QStringList m_args;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_APP_RESTARTER_H_
