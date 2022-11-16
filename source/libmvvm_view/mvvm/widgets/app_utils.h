/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
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

//! Sets up high DPI scaling.
void SetupHighDpiScaling(bool use_system_scale);

//! Sets point size of the main application font.
void SetApplicationFontSize(int point_size);

}  // namespace mvvm

#endif  // MVVM_WIDGETS_APP_UTILS_H_
