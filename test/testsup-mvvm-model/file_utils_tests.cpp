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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/utils/file_utils.h"

#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

#include <QDir>
#include <string>

using namespace mvvm;

class FileUtilsTest : public mvvm::test::FolderTest
{
public:
  FileUtilsTest() : FolderTest("FileUtilsTest") {}
};

TEST_F(FileUtilsTest, IsExists)
{
  EXPECT_TRUE(utils::IsExists(GetTestHomeDir()));
  EXPECT_FALSE(utils::IsExists(std::string()));
  EXPECT_FALSE(utils::IsExists(std::string("abc")));
}

TEST_F(FileUtilsTest, CreateDirectory)
{
  std::string dirname = GetTestHomeDir() + std::string("/") + "subdir";
  utils::Remove(dirname);

  EXPECT_TRUE(utils::CreateDirectory(dirname));
  EXPECT_TRUE(utils::IsExists(dirname));
}

TEST_F(FileUtilsTest, RemoveAll)
{
  std::string dirname = GetTestHomeDir() + std::string("/") + "subdir2";
  utils::CreateDirectory(dirname);

  EXPECT_TRUE(utils::IsExists(dirname));
  utils::RemoveAll((dirname));
  EXPECT_FALSE(utils::IsExists(dirname));
}

TEST_F(FileUtilsTest, GetFileName)
{
  std::string filename = GetTestHomeDir() + std::string("/testmodel/fileutils.cpp");
  EXPECT_EQ(utils::GetFileName(filename), "fileutils.cpp");

  std::string dirname = GetTestHomeDir() + std::string("/testmodel");
  EXPECT_EQ(utils::GetFileName(dirname), "testmodel");
}

TEST_F(FileUtilsTest, GetPathStem)
{
  std::string filename = GetTestHomeDir() + std::string("/testmodel/fileutils.test.cpp");
  EXPECT_EQ(utils::GetPathStem(filename), "fileutils.test");

  std::string dirname = GetTestHomeDir() + std::string("/testmodel");
  EXPECT_EQ(utils::GetPathStem(dirname), "testmodel");
}

TEST_F(FileUtilsTest, FindFiles)
{
  mvvm::test::CreateTextFile(GetTestHomeDir() + "/a.txt", "");
  mvvm::test::CreateTextFile(GetTestHomeDir() + "/name0.xml", "");
  mvvm::test::CreateTextFile(GetTestHomeDir() + "/name1.xml", "");

  auto found_files = utils::FindFiles(GetTestHomeDir(), ".xml");

  ASSERT_EQ(found_files.size(), 2);
  EXPECT_NE(found_files.end(), std::find(found_files.begin(), found_files.end(),
                                         utils::Join(GetTestHomeDir(), "name0.xml")));
  EXPECT_NE(found_files.end(), std::find(found_files.begin(), found_files.end(),
                                         utils::Join(GetTestHomeDir(), "name1.xml")));
}

TEST_F(FileUtilsTest, GetParentPath)
{
  // parent path of testPath() is the main test folder
  // "<build>/test_output/test_FileUtils" -> "<build>/test_output/"
  EXPECT_EQ(utils::GetParentPath(GetTestHomeDir()), mvvm::test::GetTestSuiteOutputDir());

  // "<build>/test_output/test_FileUtils/a.txt" -> "<build>/test_output/test_FileUtils/"
  mvvm::test::CreateTextFile(GetTestHomeDir() + "/a.txt", "");
  EXPECT_EQ(utils::GetParentPath(GetTestHomeDir() + "/a.txt"), GetTestHomeDir());
}

TEST_F(FileUtilsTest, IsEmpty)
{
  // creating new empty directory
  std::string dirname = GetTestHomeDir() + std::string("/") + "subdir_is_empty";
  utils::RemoveAll(dirname);
  utils::CreateDirectory(dirname);

  // it should be empty
  EXPECT_TRUE(utils::IsEmpty(dirname));

  // creating file in it, directory should be not empty
  mvvm::test::CreateTextFile(dirname + "/a.txt", "file content");
  EXPECT_FALSE(utils::IsEmpty(dirname));
  // file itself should be not empty
  EXPECT_FALSE(utils::IsEmpty(dirname + "/a.txt"));

  // creating empty file
  mvvm::test::CreateTextFile(dirname + "/a2.txt", "");
  EXPECT_TRUE(utils::IsEmpty(dirname + "/a2.txt"));
}
