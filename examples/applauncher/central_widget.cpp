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

#include "central_widget.h"

#include <mvvm/widgets/main_window_types.h>

#include <QGridLayout>
#include <QPushButton>

namespace applauncher
{

CentralWidget::CentralWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_exit_button(new QPushButton("Exit"))
    , m_restart_button(new QPushButton("Restart"))
    , m_clear_and_restart_button(new QPushButton("Clear and restart"))
{
  m_exit_button->setMaximumWidth(200);
  connect(m_exit_button, &QPushButton::clicked, this,
          [this]() { emit ExitRequest(static_cast<int>(mvvm::AppExitCode::NormalExit)); });

  m_restart_button->setMaximumWidth(200);
  connect(m_restart_button, &QPushButton::clicked, this,
          [this]() { emit ExitRequest(static_cast<int>(mvvm::AppExitCode::Restart)); });

  m_clear_and_restart_button->setMaximumWidth(200);
  connect(m_clear_and_restart_button, &QPushButton::clicked, this, [this]()
          { emit ExitRequest(static_cast<int>(mvvm::AppExitCode::CleanSettingsAndRestart)); });

  auto layout = new QGridLayout(this);
  layout->addWidget(m_exit_button);
  layout->addWidget(m_restart_button);
  layout->addWidget(m_clear_and_restart_button);
}

}  // namespace applauncher
