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

#ifndef APPLAUNCHER_CENTRAL_WIDGET_H_
#define APPLAUNCHER_CENTRAL_WIDGET_H_

#include <mvvm/widgets/main_window_types.h>

#include <QWidget>

class QPushButton;
class QLabel;
class QTimer;

namespace applauncher
{

/**
 * @brief The CentralWidget class contains a label with running counter,
 * and 3 buttons to define different ways of exiting from the application.
 */
class CentralWidget : public QWidget
{
  Q_OBJECT

public:
  explicit CentralWidget(QWidget* parent_widget = nullptr);

signals:
  void ExitRequest(int exit_request);

public:
  void UpdateLabel() const;
  void SendExitRequest(mvvm::AppExitRequest exit_request);

  QLabel* m_counter_label{nullptr};
  QTimer* m_timer{nullptr};
  QPushButton* m_exit_button{nullptr};
  QPushButton* m_restart_button{nullptr};
  QPushButton* m_clear_and_restart_button{nullptr};
};

/**
 * @brief Returns incremented value of the static counter.
 *
 * The main purpose of this function is to check that static variable get reset after application
 * launched as separate process.
 */
std::int32_t GetIncrementedCounter();

}  // namespace applauncher

#endif  // APPLAUNCHER_CENTRAL_WIDGET_H_
