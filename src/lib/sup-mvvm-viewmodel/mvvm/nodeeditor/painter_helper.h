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

#ifndef MVVM_NODEEDITOR_PAINTER_HELPER_H_
#define MVVM_NODEEDITOR_PAINTER_HELPER_H_

//! @file
//! Collection of helper functions for painting.

namespace mvvm
{

/**
 * @brief Scales pen width according to the current high-DPI settings.
 */
double ScalePenWidth(double value, double min_value = 1.0);

/**
 * @brief Scales pen width according to the current high-DPI settings.
 *
 * The value will be rounded halfway, and then min value is applied.
 * 0.1->1.0, 1.0->1.0, 1.25->1.0, 1.5->2.0
 */
double ScaleAndRoundPenWidth(double value, double min_value = 1.0);

}  // namespace mvvm

#endif  // MVVM_NODEEDITOR_PAINTER_HELPER_H_
