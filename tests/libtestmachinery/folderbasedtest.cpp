/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "folderbasedtest.h"

#include "test_utils.h"

#include "mvvm/utils/fileutils.h"

FolderBasedTest::FolderBasedTest(std::string test_home_folder)
    : m_test_home_folder(std::move(test_home_folder))
{
  mvvm::utils::CreateDirectory(GetTestHomeDirectory());
}

std::string FolderBasedTest::GetTestHomeFolderName() const
{
  return m_test_home_folder;
}

//! Returns full path to the test folder. Located in CMAKE_BINARY_DIR/test_output/<m_test_dir>.

std::string FolderBasedTest::GetTestHomeDirectory() const
{
  return testutils::GetTestSuiteOutputDir() + std::string("/") + GetTestHomeFolderName();
}

//! Returns full path to the file with given name located in test directory.

std::string FolderBasedTest::GetFilePath(const std::string &file_name) const
{
  return GetTestHomeDirectory() + std::string("/") + file_name;
}

//! Creates an empty directory in main test folder.
//! Remove recursively previous one with the same name, if exist.

std::string FolderBasedTest::CreateEmptyDir(const std::string &subdir) const
{
  auto path = mvvm::utils::Join(GetTestHomeDirectory(), subdir);
  mvvm::utils::RemoveAll(path);
  mvvm::utils::CreateDirectory(path);
  return path;
}

FolderBasedTest::~FolderBasedTest() = default;
