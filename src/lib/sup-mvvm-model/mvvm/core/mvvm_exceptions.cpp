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

#include "mvvm_exceptions.h"

namespace mvvm
{

MessageException::MessageException(std::string message) : m_message{std::move(message)} {}

const char* MessageException::what() const noexcept
{
  return m_message.c_str();
}

RuntimeException::RuntimeException(const std::string& message) : MessageException{message} {}

NullArgumentException::NullArgumentException(const std::string& message) : MessageException{message}
{
}

NotImplementedException::NotImplementedException(const std::string& message)
    : MessageException{message}
{
}

InvalidOperationException::InvalidOperationException(const std::string& message)
    : MessageException{message}
{
}

LogicErrorException::LogicErrorException(const std::string& message) : MessageException{message} {}

}  // namespace mvvm
