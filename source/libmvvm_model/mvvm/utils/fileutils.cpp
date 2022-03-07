// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/utils/fileutils.h"

#include "mvvm/core/filesystem.h"

#include <stdexcept>

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

std::string GetBaseName(const std::string& path)
{
  return std::filesystem::path(path).stem().string();
}

std::vector<std::string> FindFiles(const std::string& dirname, const std::string& ext)
{
  std::vector<std::string> result;
  for (const auto& entry : std::filesystem::directory_iterator(dirname))
  {
    const auto filenameStr = entry.path().filename().string();
    if (entry.is_regular_file() && entry.path().extension() == ext)
    {
      result.push_back(entry.path().string());
    }
  }
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
