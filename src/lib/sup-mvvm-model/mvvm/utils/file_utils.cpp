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

#include "file_utils.h"

#include <mvvm/core/filesystem.h>
#include <mvvm/core/exceptions.h>

namespace mvvm::utils
{

bool IsExists(const std::string& fileName)
{
  return std::filesystem::exists(fileName);
}

std::string Join(const std::string& part1, const std::string& part2)
{
  auto path = std::filesystem::path(part1) / std::filesystem::path(part2);
  return path.string();
}

bool CreateDirectory(const std::string& path)
{
  return std::filesystem::create_directory(path);
}

bool Remove(const std::string& path)
{
  return std::filesystem::remove(path);
}

void RemoveAll(const std::string& path)
{
  std::filesystem::remove_all(path);
}

std::string GetFileName(const std::string& path)
{
  return std::filesystem::path(path).filename();
}

std::string GetPathStem(const std::string& path)
{
  return std::filesystem::path(path).stem().string();
}

std::vector<std::string> FindFiles(const std::string& dirname, const std::string& ext)
{
  std::vector<std::string> result;
#if INCLUDE_STD_FILESYSTEM_EXPERIMENTAL == 0
  for (const auto& entry : std::filesystem::directory_iterator(dirname))
  {
    const auto filenameStr = entry.path().filename().string();
    if (entry.is_regular_file() && entry.path().extension() == ext)
    {
      result.push_back(entry.path().string());
    }
  }
#else
  throw RuntimeException("Sorry, your compiler is too old to use <filesystem>");
#endif

  return result;
}

std::string GetParentPath(const std::string& path)
{
  return std::filesystem::path(path).parent_path().string();
}

bool IsEmpty(const std::string& path)
{
  return std::filesystem::is_empty(path);
}

}  // namespace mvvm::utils
