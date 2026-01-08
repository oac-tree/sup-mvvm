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

#ifndef MVVM_WIDGETS_MAIN_WINDOW_TYPES_H_
#define MVVM_WIDGETS_MAIN_WINDOW_TYPES_H_

#include <cstdint>

namespace mvvm
{

/**
 * @brief The AppExitCode enum defines exit codes of the application.
 */
enum class AppExitCode : std::uint8_t
{
  NormalExit,
  Restart,
  CleanSettingsAndRestart
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_MAIN_WINDOW_TYPES_H_
