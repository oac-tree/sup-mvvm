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

#ifndef MVVM_CORE_EXCEPTIONS_H_
#define MVVM_CORE_EXCEPTIONS_H_

//! @file
//! Defines collection of MVVM specific exceptions.

#include <exception>
#include <string>

namespace mvvm
{

//! Base Exception class with message.

class MessageException : public std::exception
{
public:
  explicit MessageException(std::string message);
  const char* what() const noexcept override;

private:
  std::string message;
};

//! Generic error happening at runtime.

class RuntimeException : public MessageException
{
public:
  explicit RuntimeException(const std::string& message);
};

//! Exception is thrown when no such key exists in a map.

class KeyNotFoundException : public MessageException
{
public:
  explicit KeyNotFoundException(const std::string& message);
};

//! Exception is thrown when such key already exists in the map.

class ExistingKeyException : public MessageException
{
public:
  explicit ExistingKeyException(const std::string& message);
};

//! Exception is thrown when the argument is nullptr.

class NullArgumentException : public MessageException
{
public:
  explicit NullArgumentException(const std::string& message);
};

//! Exception is thrown when code is not implemented.

class NotImplementedException : public MessageException
{
public:
  explicit NotImplementedException(const std::string& message);
};

//! Exception is thrown on attempt to perform ivalid operation.

class InvalidOperationException : public MessageException
{
public:
  explicit InvalidOperationException(const std::string& message);
};

//! Exception is thrown when faulty logic is discovered.
class LogicErrorException : public MessageException
{
public:
  explicit LogicErrorException(const std::string& message);
};

}  // namespace mvvm

#endif  // MVVM_CORE_EXCEPTIONS_H_
