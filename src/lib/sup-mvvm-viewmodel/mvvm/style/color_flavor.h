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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_STYLE_COLOR_FLAVOR_H_
#define MVVM_STYLE_COLOR_FLAVOR_H_

#include <cstdint>

namespace mvvm
{

/**
 * @brief The ColorFlavor enum defines the dark/light type of the desktop environment.
 *
 * It is used in the context, when we want to find some style element (i.e. icon or color) suitable
 * for current desktop theme.
 */
enum class ColorFlavor : std::uint8_t
{
  kAuto,           //!< dark or light will be automatically deduced from current theme
  kForDarkThemes,  //!< intended for dark background
  kForLightThemes  //!< intended for light background
};

}  // namespace mvvm

#endif  // MVVM_STYLE_COLOR_FLAVOR_H_
