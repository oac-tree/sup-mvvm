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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_WIDGETS_APPEARANCE_HELPER_H_
#define MVVM_WIDGETS_APPEARANCE_HELPER_H_

//! @file appearance_helper.h
//! Collection of style and color related helper functions.

#include <QString>

class QColor;

namespace mvvm
{

/**
 * @brief Returns a string representing a borderless style for QToolButton.
 *
 * @param background_color A color of the widget under the button.
 */
QString GetFlatButtonStyleString(const QColor& background_color);

}  // namespace mvvm::utils

#endif  // MVVM_WIDGETS_APPEARANCE_HELPER_H_
