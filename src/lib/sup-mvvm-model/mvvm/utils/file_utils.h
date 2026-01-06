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

#ifndef MVVM_UTILS_FILE_UTILS_H_
#define MVVM_UTILS_FILE_UTILS_H_

#include <string>
#include <vector>

namespace mvvm::utils
{

//! Returns true if file exists.
bool IsExists(const std::string& filename);

//! Joins two path elements into the path.
std::string Join(const std::string& part1, const std::string& part2);

//! Create directory, parent directory must exist. If path resolves to existing directory,
//! no error reported.
bool CreateDirectory(const std::string& path);

//! Removes file or empty directory.
bool Remove(const std::string& path);

//! Removes directory with all its content.
void RemoveAll(const std::string& path);

//! Provide the filename without a path (aka basename).
std::string GetFileName(const std::string& path);

//! Provide the filename (no path, no last extension).
std::string GetPathStem(const std::string& path);

//! Returns list of files with given extention found in given directory.
std::vector<std::string> FindFiles(const std::string& dirname, const std::string& ext);

//! Returns the path to the parent directory.
std::string GetParentPath(const std::string& path);

//! Returns true if the file indicated by 'path' refers to empty file or directory.
bool IsEmpty(const std::string& path);

}  // namespace mvvm::utils

#endif  // MVVM_UTILS_FILE_UTILS_H_
