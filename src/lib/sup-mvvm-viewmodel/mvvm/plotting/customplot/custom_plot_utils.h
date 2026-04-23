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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_CUSTOM_PLOT_UTILS_H_
#define MVVM_PLOTTING_CUSTOMPLOT_CUSTOM_PLOT_UTILS_H_

class QCPAxis;

namespace mvvm::utils
{

//! Switch axis to logarithmic scale mode.
void SetLogarithmicScale(QCPAxis* axis, bool is_log_scale);

}  // namespace mvvm::utils

#endif  // MVVM_PLOTTING_CUSTOMPLOT_CUSTOM_PLOT_UTILS_H_
