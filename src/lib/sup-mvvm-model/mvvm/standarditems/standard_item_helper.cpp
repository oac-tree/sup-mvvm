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

#include "standard_item_helper.h"

namespace mvvm
{

int GetDefaultAxisTitlePointSize()
{
  // TODO provide the way to make it system dependent
  const int kDefaultPointSize = 10;

  // The difficulty here is that we do not have a dependency on QApplication, and still want to
  // provide a reasonable default size. Probably this constant should rely on global reference,
  // defined at startup time already in the main.cpp right after QApplication starts. See also
  // SystemPointSize() to get the idea.

  return kDefaultPointSize;
}

std::string GetDefaultAxisFontFamily()
{
  const std::string kDefaultTitleFamily = "Noto Sans";

  return kDefaultTitleFamily;
}

}  // namespace mvvm
