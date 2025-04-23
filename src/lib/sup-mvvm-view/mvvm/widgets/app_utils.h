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

#ifndef MVVM_WIDGETS_APP_UTILS_H_
#define MVVM_WIDGETS_APP_UTILS_H_

//! @file app_utils.h
//! Collection of utils for main() function.

#include <string>

namespace mvvm::utils
{

/**
 * @brief Returns multiline-string describing system environment related to the user desktop.
 */
std::string GetDesktopInfo();

/**
 * @brief Sets up high DPI scaling.
 *
 * Set up Qt environment to look good on 4K monitors.  If scale_from_environment is false (default
 * case), will explicitly ignore the possible presence of environment variables QT_SCALE_FACTOR,
 * QT_SCREEN_SCALE_FACTORS and QT_AUTO_SCREEN_SCALE_FACTOR. This is recommended way since our GUI
 * is perfect and always looks good on any monitor.
 *
 * When scale_from_environment is true, we will let Qt perform extra scaling based on QT_
 * environment variables. Please note, that this is not recommended, since all those variables have
 * been invented by Qt to support old GUIs without any idea about 4K.
 *
 * @param scale_from_environment Scale using Qt environment variables when true.
 */
void SetupHighDpiScaling(bool scale_from_environment = false);

/**
 * @brief Sets application main font point size.
 *
 * @param point_size The font size in points.
 */
void SetApplicationFontSize(int point_size);

}  // namespace mvvm::utils

#endif  // MVVM_WIDGETS_APP_UTILS_H_
