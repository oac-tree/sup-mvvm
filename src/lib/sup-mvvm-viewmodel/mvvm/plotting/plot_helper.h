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

#ifndef MVVM_PLOTTING_PLOT_HELPER_H_
#define MVVM_PLOTTING_PLOT_HELPER_H_

//! @file
//! Collection of helper functions for plotting in Qt.

#include <Qt>

namespace AnimationOptions{

}

namespace mvvm
{

class PenItem;

/**
 * @brief Returns Qt's pen style from given item
 */
Qt::PenStyle GetQtPenStyle(mvvm::PenItem* item);

}  // namespace mvvm

#endif  // MVVM_PLOTTING_PLOT_HELPER_H_
