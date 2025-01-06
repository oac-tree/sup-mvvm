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

#ifndef MVVM_SIGNALS_SIGNAL_SLOT_FWD_H_
#define MVVM_SIGNALS_SIGNAL_SLOT_FWD_H_

//! @file
//! Forward declaration of signal/slot classes.

namespace lsignal
{
class slot;
}

namespace mvvm
{

using Slot = lsignal::slot;

}  // namespace mvvm

#endif  // MVVM_SIGNALS_SIGNAL_SLOT_FWD_H_
