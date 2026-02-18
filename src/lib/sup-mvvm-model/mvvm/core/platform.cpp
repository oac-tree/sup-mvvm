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

#include "platform.h"

#include <cstdlib>
#include <string>

namespace mvvm
{

namespace
{
constexpr auto kQtPlatformEnvironmentVariable = "QT_QPA_PLATFORM";
constexpr auto kOffscreenPluginName = "offscreen";
}  // namespace

bool IsLinuxHost()
{
#ifdef __linux__
  return true;
#else
  return false;
#endif
}

bool IsWindowsHost()
{
#if defined(_WIN32) || defined(__CYGWIN__)
  return true;
#else
  return false;
#endif
}

bool IsMacHost()
{
#if defined(__APPLE__) && defined(__MACH__)
  return true;
#else
  return false;
#endif
}

bool IsHeadless()
{
  if (std::getenv(kQtPlatformEnvironmentVariable) != nullptr)
  {
    return std::string(std::getenv(kQtPlatformEnvironmentVariable)) == kOffscreenPluginName;
  }
  return false;
}

void SetHeadless(bool value)
{
  if (value)
  {
    setenv(kQtPlatformEnvironmentVariable, kOffscreenPluginName, 1);
  }
  else
  {
    unsetenv(kQtPlatformEnvironmentVariable);
  }
}

}  // namespace mvvm
