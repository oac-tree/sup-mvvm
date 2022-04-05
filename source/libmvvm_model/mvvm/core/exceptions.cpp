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

#include "mvvm/core/exceptions.h"

namespace mvvm
{

MessageException::MessageException(std::string message_) : message{std::move(message_)} {}

const char* MessageException::what() const noexcept
{
  return message.c_str();
}

RuntimeError::RuntimeError(const std::string& message) : MessageException{message} {}

}  // namespace mvvm
