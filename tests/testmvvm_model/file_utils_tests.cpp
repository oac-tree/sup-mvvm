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

#include "mvvm/utils/file_utils.h"

#include "folder_based_tests.h"
#include "test_utils.h"

#include <gtest/gtest.h>

#include <QDir>
#include <stdexcept>
#include <string>

using namespace mvvm;

class FileUtilsTest : public FolderBasedTest
{
public:
  FileUtilsTest() : FolderBasedTest("test_FileUtils") {}
};

TEST_F(FileUtilsTest, exists)
{
  EXPECT_TRUE(utils::IsExists(GetTestHomeDir()));
  EXPECT_FALSE(utils::IsExists(std::string()));
  EXPECT_FALSE(utils::IsExists(std::string("abc")));
}

TEST_F(FileUtilsTest, create_directory)
{
  std::string dirname = GetTestHomeDir() + std::string("/") + "subdir";
  utils::Remove(dirname);

  EXPECT_TRUE(utils::CreateDirectory(dirname));
  EXPECT_TRUE(utils::IsExists(dirname));
}

TEST_F(FileUtilsTest, remove_all)
{
  std::string dirname = GetTestHomeDir() + std::string("/") + "subdir2";
  utils::CreateDirectory(dirname);

  EXPECT_TRUE(utils::IsExists(dirname));
  utils::RemoveAll((dirname));
  EXPECT_FALSE(utils::IsExists(dirname));
}

TEST_F(FileUtilsTest, base_name)
{
  std::string filename = GetTestHomeDir() + std::string("/testmodel/fileutils.test.cpp");
  std::string base_name = utils::GetBaseName(filename);

  EXPECT_EQ("fileutils.test", base_name);
}

TEST_F(FileUtilsTest, FindFiles)
{
  testutils::CreateTextFile(GetTestHomeDir() + "/a.txt", "");
  testutils::CreateTextFile(GetTestHomeDir() + "/name0.xml", "");
  testutils::CreateTextFile(GetTestHomeDir() + "/name1.xml", "");

  auto found_files = utils::FindFiles(GetTestHomeDir(), ".xml");

  ASSERT_EQ(found_files.size(), 2);
  EXPECT_NE(found_files.end(), std::find(found_files.begin(), found_files.end(),
                                         utils::Join(GetTestHomeDir(), "name0.xml")));
  EXPECT_NE(found_files.end(), std::find(found_files.begin(), found_files.end(),
                                         utils::Join(GetTestHomeDir(), "name1.xml")));
}

TEST_F(FileUtilsTest, parent_path)
{
  // parent path of testPath() is the main test folder
  // "<build>/test_output/test_FileUtils" -> "<build>/test_output/"
  EXPECT_EQ(utils::GetParentPath(GetTestHomeDir()), testutils::GetTestSuiteOutputDir());

  // "<build>/test_output/test_FileUtils/a.txt" -> "<build>/test_output/test_FileUtils/"
  testutils::CreateTextFile(GetTestHomeDir() + "/a.txt", "");
  EXPECT_EQ(utils::GetParentPath(GetTestHomeDir() + "/a.txt"), GetTestHomeDir());
}

TEST_F(FileUtilsTest, is_empty)
{
  // creating new empty directory
  std::string dirname = GetTestHomeDir() + std::string("/") + "subdir_is_empty";
  utils::RemoveAll(dirname);
  utils::CreateDirectory(dirname);

  // it should be empty
  EXPECT_TRUE(utils::IsEmpty(dirname));

  // creating file in it, directory should be not empty
  testutils::CreateTextFile(dirname + "/a.txt", "file content");
  EXPECT_FALSE(utils::IsEmpty(dirname));
  // file itself should be not empty
  EXPECT_FALSE(utils::IsEmpty(dirname + "/a.txt"));

  // creating empty file
  testutils::CreateTextFile(dirname + "/a2.txt", "");
  EXPECT_TRUE(utils::IsEmpty(dirname + "/a2.txt"));
}
