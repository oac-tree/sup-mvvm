/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_WIDGETS_APP_UTILS_H_
#define MVVM_WIDGETS_APP_UTILS_H_

//! @file app_utils.h
//! Collection of utils for main() function.

#include <string>

namespace mvvm::utils
{

//! Returns multiline-string describing system environment related to the user desktop.
std::string GetDesktopInfo();

//! Sets up high DPI scaling. If \it scale_from_environment is false (default case),
//! will explicitly ignore Qt's environment variables QT_SCALE_FACTOR, QT_SCREEN_SCALE_FACTORS and
//! QT_AUTO_SCREEN_SCALE_FACTOR.
void SetupHighDpiScaling(bool scale_from_environment = false);

//! Sets point size of the main application font.
void SetApplicationFontSize(int point_size);

}  // namespace mvvm::utils

#endif  // MVVM_WIDGETS_APP_UTILS_H_
