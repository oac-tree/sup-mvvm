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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_SIGNALS_SIGNAL_SLOT_H_
#define MVVM_SIGNALS_SIGNAL_SLOT_H_

//! @file
//! Defines signal/slot types. For the moment relies on `lsignal` library, see
//! lsignal.h for copyright.

#include <mvvm/signals/lsignal.h>
#include <mvvm/signals/signal_slot_fwd.h>

namespace mvvm
{

template <typename T>
using Signal = lsignal::signal<T>;

using Connection = lsignal::connection;
}  // namespace mvvm

#endif  // MVVM_SIGNALS_SIGNAL_SLOT_H_
