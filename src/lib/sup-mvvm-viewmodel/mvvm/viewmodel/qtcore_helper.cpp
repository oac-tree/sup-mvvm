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

#include "qtcore_helper.h"

#include <QByteArray>
#include <QDataStream>
#include <QIODevice>

namespace mvvm::utils
{

QStringList GetStringList(const std::vector<std::string>& vec)
{
  QStringList result;
  std::transform(vec.begin(), vec.end(), std::back_inserter(result),
                 [](const auto& str) { return QString::fromStdString(str); });
  return result;
}

std::vector<std::string> GetStdStringVector(const QStringList& string_list)
{
  std::vector<std::string> result;
  std::transform(string_list.begin(), string_list.end(), std::back_inserter(result),
                 [](const auto& str) { return str.toStdString(); });
  return result;
}

QByteArray GetByteArray(const QStringList& data)
{
  QByteArray byteArray;
  QDataStream out(&byteArray, QIODevice::WriteOnly);
  out << data;
  return byteArray;
}

QStringList GetStringList(const QByteArray& byteArray)
{
  QByteArray array = byteArray;
  QStringList result;
  QDataStream in(&array, QIODevice::ReadOnly);
  in >> result;
  return result;
}

}  // namespace mvvm::utils
