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

#ifndef MVVM_SIGNALS_SIGNAL_SLOT_H_
#define MVVM_SIGNALS_SIGNAL_SLOT_H_

//! @file signalslot.h
//! Defines signal/slot types. For the moment relies on `lsignal` library, see
//! lsignal.h for copyright.

#include <mvvm/signals/lsignal.h>

#include <vector>
namespace mvvm
{
template <typename T>
using vector = std::vector<T>;

template <typename T>
using Signal = lsignal::signal<T>;

using Slot = lsignal::slot;
using Connection = lsignal::connection;
}  // namespace mvvm

#endif  // MVVM_SIGNALS_SIGNAL_SLOT_H_
