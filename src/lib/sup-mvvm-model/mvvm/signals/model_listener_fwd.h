/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_SIGNALS_MODEL_LISTENER_FWD_H_
#define MVVM_SIGNALS_MODEL_LISTENER_FWD_H_

//! @file
//! Trick to have both forward declaration, and default template parameter in client libraries. This
//! file is also included in model_listener.h
//!
//! See discussion
//! https://stackoverflow.com/questions/1792253/default-template-parameters-with-forward-declaration
//! https://stackoverflow.com/questions/20108568/c-forward-declare-using-directive

#include <mvvm/model/model_fwd.h>

namespace mvvm
{

template <typename ModelT = ISessionModel>
class ModelListener;

}  // namespace mvvm

#endif  // MVVM_SIGNALS_MODEL_LISTENER_FWD_H_
