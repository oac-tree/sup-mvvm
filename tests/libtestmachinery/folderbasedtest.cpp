// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "folderbasedtest.h"

#include "test_utils.h"

#include <filesystem>

FolderBasedTest::FolderBasedTest(std::string folder_name) : m_folder_name(std::move(folder_name))
{
  std::filesystem::create_directory(GetTestDirectory());
}

//! Returns full path to the test folder. Located in CMAKE_BINARY_DIR/test_output/<m_test_dir>.

std::string FolderBasedTest::GetTestDirectory() const
{
  return TestUtils::GetTestOutputDir() + std::string("/") + m_folder_name;
}

//! Returns full path to the file with

std::string FolderBasedTest::GetFilePath(const std::string &file_name) const
{
  return GetTestDirectory() + std::string("/") + file_name;
}

FolderBasedTest::~FolderBasedTest() = default;
