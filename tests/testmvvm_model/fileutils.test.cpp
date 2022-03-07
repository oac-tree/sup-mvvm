/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/utils/fileutils.h"

#include "folderbasedtest.h"
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
  EXPECT_TRUE(utils::exists(GetTestDirectory()));
  EXPECT_FALSE(utils::exists(std::string()));
  EXPECT_FALSE(utils::exists(std::string("abc")));
}

TEST_F(FileUtilsTest, create_directory)
{
  std::string dirname = GetTestDirectory() + std::string("/") + "subdir";
  utils::remove(dirname);

  EXPECT_TRUE(utils::create_directory(dirname));
  EXPECT_TRUE(utils::exists(dirname));
}

TEST_F(FileUtilsTest, remove_all)
{
  std::string dirname = GetTestDirectory() + std::string("/") + "subdir2";
  utils::create_directory(dirname);

  EXPECT_TRUE(utils::exists(dirname));
  utils::remove_all((dirname));
  EXPECT_FALSE(utils::exists(dirname));
}

TEST_F(FileUtilsTest, base_name)
{
  std::string filename = GetTestDirectory() + std::string("/testmodel/fileutils.test.cpp");
  std::string base_name = utils::base_name(filename);

  EXPECT_EQ("fileutils.test", base_name);
}

TEST_F(FileUtilsTest, FindFiles)
{
  testutils::CreateTextFile(GetTestDirectory() + "/a.txt", "");
  testutils::CreateTextFile(GetTestDirectory() + "/name0.json", "");
  testutils::CreateTextFile(GetTestDirectory() + "/name1.json", "");

  auto found_files = utils::FindFiles(GetTestDirectory(), ".json");

  ASSERT_EQ(found_files.size(), 2);
  EXPECT_NE(found_files.end(), std::find(found_files.begin(), found_files.end(),
                                         utils::join(GetTestDirectory(), "name0.json")));
  EXPECT_NE(found_files.end(), std::find(found_files.begin(), found_files.end(),
                                         utils::join(GetTestDirectory(), "name1.json")));
}

TEST_F(FileUtilsTest, parent_path)
{
  // parent path of testPath() is the main test folder
  // "<build>/test_output/test_FileUtils" -> "<build>/test_output/"
  EXPECT_EQ(utils::parent_path(GetTestDirectory()), testutils::GetTestSuiteOutputDir());

  // "<build>/test_output/test_FileUtils/a.txt" -> "<build>/test_output/test_FileUtils/"
  testutils::CreateTextFile(GetTestDirectory() + "/a.txt", "");
  EXPECT_EQ(utils::parent_path(GetTestDirectory() + "/a.txt"), GetTestDirectory());
}

TEST_F(FileUtilsTest, is_empty)
{
  // creating new empty directory
  std::string dirname = GetTestDirectory() + std::string("/") + "subdir_is_empty";
  utils::remove_all(dirname);
  utils::create_directory(dirname);

  // it should be empty
  EXPECT_TRUE(utils::is_empty(dirname));

  // creating file in it, directory should be not empty
  testutils::CreateTextFile(dirname + "/a.txt", "file content");
  EXPECT_FALSE(utils::is_empty(dirname));
  // file itself should be not empty
  EXPECT_FALSE(utils::is_empty(dirname + "/a.txt"));

  // creating empty file
  testutils::CreateTextFile(dirname + "/a2.txt", "");
  EXPECT_TRUE(utils::is_empty(dirname + "/a2.txt"));
}
