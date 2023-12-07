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

#include "plot_helper.h"

#include <mvvm/standarditems/plottable_items.h>

namespace mvvm
{

Qt::PenStyle GetQtPenStyle(mvvm::PenItem* item)
{
  // our ComboProperty for pens coincide with Qt definition
  return static_cast<Qt::PenStyle>(item->GetStyleIndex());
}

}  // namespace mvvm
