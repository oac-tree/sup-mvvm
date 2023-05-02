/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_WIDGETS_LAYOUT_UTILS_H_
#define MVVM_WIDGETS_LAYOUT_UTILS_H_

#include "mvvm/view_export.h"

class QLayout;
class QGridLayout;
class QWidget;

//! @namespace LayoutUtils
//! @brief Utility functions to add/remove widgets to the layout on the fly.
//! Taken from https://stackoverflow.com/questions/5395266/removing-widgets-from-qgridlayout
//! Caveat: according to explanations given, grid layouts can only grow and never shrink.

namespace mvvm
{

//! Removes content from box layout.
MVVM_VIEW_EXPORT void ClearLayout(QLayout* layout, bool deleteWidgets = true);

//! Removes row from grid layout (important: doesn't change row count).
MVVM_VIEW_EXPORT void RemoveRow(QGridLayout* layout, int row, bool deleteWidgets = true);

//! Removes column from grid layout.
MVVM_VIEW_EXPORT void RemoveColumn(QGridLayout* layout, int column, bool deleteWidgets = true);

//! Clear layout completely.
MVVM_VIEW_EXPORT void ClearGridLayout(QGridLayout* layout, bool deleteWidgets = true);

//! Returns empty widget to occupy place in layout.
MVVM_VIEW_EXPORT QWidget* CreatePlaceHolder();

}  // namespace mvvm

#endif  // MVVM_WIDGETS_LAYOUT_UTILS_H_
