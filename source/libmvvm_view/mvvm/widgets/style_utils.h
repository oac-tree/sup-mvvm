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

#ifndef MVVM_WIDGETS_STYLE_UTILS_H_
#define MVVM_WIDGETS_STYLE_UTILS_H_

//! @file style_utils.h
//! Collection of various style-related utils.

#include <string>

class QSize;
class QIcon;
class QTreeView;

namespace mvvm::utils
{

//! Returns characteristic UI size scaled by a given factor.
//! Characteristic size corresponds to the width of the letter 'M', expressed in units of Qt's
//! own virtual coordinate system. The user is expected to express all widget sizes in these units.
int UnitSize(double scale = 1.0);

//! Returns default point size of the application's font.
int AppFontSize();

//! Size of tolbar icons.
QSize ToolBarIconSize();

//! Size of tolbar icons.
QSize NarrowToolBarIconSize();

QIcon GetIcon(const std::string& icon_name);

//! Sets Breeze style for the tree to use in property editors.
//! Used for X-environments that do not have Breeze theme.
void SetBreezePropertyStyle(QTreeView* tree);

}  // namespace mvvm::utils

#endif  // MVVM_WIDGETS_STYLE_UTILS_H_
