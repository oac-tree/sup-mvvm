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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_CUSTOM_PLOT_UTILS_H_
#define MVVM_PLOTTING_CUSTOMPLOT_CUSTOM_PLOT_UTILS_H_

#include <mvvm/view_export.h>

class QCPColorScale;
class QCPAxis;

namespace mvvm::utils
{

//! Switch axis to logarithmic scale mode.
MVVM_VIEW_EXPORT void SetLogarithmicScale(QCPColorScale* axis, bool is_log_scale);

//! Switch axis to logarithmic scale mode.
MVVM_VIEW_EXPORT void SetLogarithmicScale(QCPAxis* axis, bool is_log_scale);

}  // namespace mvvm::utils

#endif  // MVVM_PLOTTING_CUSTOMPLOT_CUSTOM_PLOT_UTILS_H_
