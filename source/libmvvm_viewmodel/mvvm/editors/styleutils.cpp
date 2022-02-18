/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/styleutils.h"

namespace mvvm::style
{

int DefaultPixmapSize()
{
  const int default_pixmap_size = 16;
  return default_pixmap_size;
}

int DefaultInfoBarHeight()
{
  const int default_info_bar_height = 24;
  return default_info_bar_height;
}

int DefaultInfoBarTextSize()
{
  const int default_info_bar_text_saize = 8;
  return default_info_bar_text_saize;
}

}  // namespace mvvm::style
