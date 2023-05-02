/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_UTILS_FILE_UTILS_H_
#define MVVM_UTILS_FILE_UTILS_H_

#include <mvvm/model_export.h>

#include <string>
#include <vector>

namespace mvvm::utils
{

//! Returns true if file exists.
MVVM_MODEL_EXPORT bool IsExists(const std::string& filename);

//! Joins two path elements into the path.
MVVM_MODEL_EXPORT std::string Join(const std::string& part1, const std::string& part2);

//! Create directory, parent directory must exist. If path resolves to existing directory,
//! no error reported.
MVVM_MODEL_EXPORT bool CreateDirectory(const std::string& path);

//! Removes file or empty directory.
MVVM_MODEL_EXPORT bool Remove(const std::string& path);

//! Removes directory with all its content.
MVVM_MODEL_EXPORT void RemoveAll(const std::string& path);

//! Provide the filename without a path (aka basename).
MVVM_MODEL_EXPORT std::string GetFileName(const std::string& path);

//! Provide the filename (no path, no last extension).
MVVM_MODEL_EXPORT std::string GetPathStem(const std::string& path);

//! Returns list of files with given extention found in given directory.
MVVM_MODEL_EXPORT std::vector<std::string> FindFiles(const std::string& dirname,
                                                     const std::string& ext);

//! Returns the path to the parent directory.
MVVM_MODEL_EXPORT std::string GetParentPath(const std::string& path);

//! Returns true if the file indicated by 'path' refers to empty file or directory.
MVVM_MODEL_EXPORT bool IsEmpty(const std::string& path);

}  // namespace mvvm::utils

#endif  // MVVM_UTILS_FILE_UTILS_H_
