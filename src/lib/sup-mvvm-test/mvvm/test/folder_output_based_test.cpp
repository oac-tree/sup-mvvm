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

#include "folder_output_based_test.h"

#include <mvvm/utils/file_utils.h>

namespace mvvm::test
{

FolderOutputBasedTest::FolderOutputBasedTest(std::string test_suite_output_dir,
                                             std::string test_home_dirname)
    : m_test_suite_output_dir(std::move(test_suite_output_dir))
    , m_test_home_dirname(std::move(test_home_dirname))
{
  mvvm::utils::CreateDirectory(GetTestHomeDir());
}

FolderOutputBasedTest::~FolderOutputBasedTest() = default;

std::string FolderOutputBasedTest::GetTestHomeDirName() const
{
  return m_test_home_dirname;
}

std::string FolderOutputBasedTest::GetTestHomeDir() const
{
  return m_test_suite_output_dir + std::string("/") + GetTestHomeDirName();
}

std::string FolderOutputBasedTest::GetFilePath(const std::string &file_name) const
{
  return GetTestHomeDir() + std::string("/") + file_name;
}

std::string FolderOutputBasedTest::CreateEmptyDir(const std::string &subdir) const
{
  auto path = mvvm::utils::Join(GetTestHomeDir(), subdir);
  mvvm::utils::RemoveAll(path);
  mvvm::utils::CreateDirectory(path);
  return path;
}

}  // namespace mvvm::test
