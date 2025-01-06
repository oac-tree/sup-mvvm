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

#ifndef MVVM_UTILS_THREADSAFE_QUEUE_H_
#define MVVM_UTILS_THREADSAFE_QUEUE_H_

#include <mvvm/utils/threadsafe_container_adapter.h>

#include <queue>

namespace mvvm
{

//! Thread-safe queue.

template <typename T>
class threadsafe_queue : public threadsafe_container_adapter<std::queue<T>>
{
};

}  // namespace mvvm

#endif  // MVVM_UTILS_THREADSAFE_QUEUE_H_
