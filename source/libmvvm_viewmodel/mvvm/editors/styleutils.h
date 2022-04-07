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

#ifndef MVVM_EDITORS_STYLEUTILS_H
#define MVVM_EDITORS_STYLEUTILS_H

#include "mvvm/viewmodel_export.h"

namespace mvvm::style
{

//! Returns int value corresponding to pixmap in standard Qt table/tree decorations.
MVVM_VIEWMODEL_EXPORT int DefaultPixmapSize();

//! Returns default height of info bar
MVVM_VIEWMODEL_EXPORT int DefaultInfoBarHeight();

//! Returns default size of text on info bar.
MVVM_VIEWMODEL_EXPORT int DefaultInfoBarTextSize();

}  // namespace mvvm::style

#endif  // MVVM_EDITORS_STYLEUTILS_H
