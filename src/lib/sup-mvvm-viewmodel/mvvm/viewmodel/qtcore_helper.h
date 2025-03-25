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

#ifndef MVVM_VIEWMODEL_QTCORE_HELPER_H_
#define MVVM_VIEWMODEL_QTCORE_HELPER_H_

//! @file
//! Collection of helper functions related to Qt core object conversion.

#include <QStringList>
#include <string>
#include <vector>

namespace mvvm::utils
{

/**
 * @brief Converts vector of strings to QStringList.
 */
QStringList GetStringList(const std::vector<std::string>& vec);

/**
 * @brief Converts string list to std vector.
 */
std::vector<std::string> GetStdStringVector(const QStringList& string_list);

/**
 * @brief Converts vector of strings to byte array.
 */
QByteArray GetByteArray(const QStringList& data);

/**
 * @brief Converts byte array to vector of strings.
 */
QStringList GetStringList(const QByteArray& byteArray);

}  // namespace mvvm::utils

#endif  // MVVM_VIEWMODEL_QTCORE_HELPER_H_
