/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_TEST_TEST_HELPER_H_
#define MVVM_TEST_TEST_HELPER_H_

#include <QSignalSpy>

QT_BEGIN_NAMESPACE
inline void PrintTo(const QString& qString, ::std::ostream* os)
{
  *os << qUtf8Printable(qString);
}
QT_END_NAMESPACE

namespace mvvm::test
{

/**
 * @brief Creates string representing XML document. User provided body will be enclosed between
 * standard header and footer.
 */
std::string CreateXMLDocumentString(const std::string& body);

/**
 * @brief Returns text file content.
 */
std::string GetTextFileContent(const std::string& file_name);

/**
 * @brief Create ASCII file with given content.
 */
void CreateTextFile(const std::string& file_name, const std::string& content);

/**
 * @brief Helper function that retrieves an object from QSignalSpy arguments.
 *
 * It is assumed that there was at least one signal received, and that the signal has only one
 * argument. After retrieving the argument, it is removed from the list of received signals.
 * Always takes first signal from the list.
 */
template <typename T>
T TakeValue(QSignalSpy& signal_spy)
{
  if (signal_spy.count() >= 1)
  {
    auto arguments = signal_spy.takeFirst();
    if (arguments.size() == 1)
    {
      return arguments.at(0).value<T>();
    }
  }
  return {};
}

/**
 * @brief Checks if given item can be casted to desired type.
 */
template <typename DesiredT, typename ItemT>
bool CanCast(const ItemT* item)
{
  return dynamic_cast<const DesiredT*>(item) != nullptr;
}

/**
 * @brief Checks if clone method is implemented.
 */
template <typename T>
bool IsCloneImplemented()
{
  // We expect that the specified object can be created, cloned, and the result of clone can
  // be casted to the object type itself.
  T item;
  auto clone = item.Clone();
  return CanCast<T>(clone.get());
}

}  // namespace mvvm::test

#endif  // MVVM_TEST_TEST_HELPER_H_
