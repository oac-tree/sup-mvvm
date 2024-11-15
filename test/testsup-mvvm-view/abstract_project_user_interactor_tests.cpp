/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/project/abstract_project_user_interactor.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/folder_test.h>

namespace mvvm
{

/**
 * @brief Tests for AbstractProjectUserInteractor class.
 */
class AbstractProjectUserInteractorTest : public mvvm::test::FolderTest
{
public:
  AbstractProjectUserInteractorTest() : mvvm::test::FolderTest("test_AbstractProjectUserInteractor")
  {
  }

  /**
   * @brief The MockInteractor class implement abstract interactor to test method call.
   */
  class MockInteractor : public AbstractProjectUserInteractor
  {
  public:
    MockInteractor() : AbstractProjectUserInteractor("MockInteractor", nullptr) {};

    MOCK_METHOD(mvvm::SaveChangesAnswer, OnSaveCurrentChangesRequest, (), (const));

    MOCK_METHOD(std::string, GetNewProjectPathImpl, (), (const));
    MOCK_METHOD(std::string, GetExistingProjectPathImpl, (), (const));
  };
};

TEST_F(AbstractProjectUserInteractorTest, InitialState)
{
  const MockInteractor mock_interactor;
  EXPECT_TRUE(mock_interactor.GetCurrentWorkdir().empty());
}

TEST_F(AbstractProjectUserInteractorTest, GetNewProjectPath)
{
  // creating text files in "<build>/test_output/test_RecentProjectPath"
  auto path1 = GetFilePath("a1.txt");
  const std::string empty_path;

  MockInteractor mock_interactor;

  // setting mock method to return path1 as user choice
  ON_CALL(mock_interactor, GetNewProjectPathImpl()).WillByDefault(::testing::Return(path1));

  EXPECT_CALL(mock_interactor, GetNewProjectPathImpl());

  EXPECT_EQ(mock_interactor.GetNewProjectPath(), path1);
  EXPECT_EQ(mock_interactor.GetCurrentWorkdir(), GetTestHomeDir());

  // setting mock method to return empty path as user choice
  ON_CALL(mock_interactor, GetNewProjectPathImpl()).WillByDefault(::testing::Return(empty_path));
  EXPECT_CALL(mock_interactor, GetNewProjectPathImpl());

  // user interactor remembers last workdir from meaningful paths, so empty path shouldn't change
  // workdir
  EXPECT_EQ(mock_interactor.GetNewProjectPath(), empty_path);
  EXPECT_EQ(mock_interactor.GetCurrentWorkdir(), GetTestHomeDir());
}

TEST_F(AbstractProjectUserInteractorTest, GetExistingProjectPath)
{
  // creating text files in "<build>/test_output/test_RecentProjectPath"
  auto path1 = GetFilePath("a2.txt");

  const std::string empty_path;

  MockInteractor mock_interactor;

  // setting mock method to return path1 as user choice
  ON_CALL(mock_interactor, GetExistingProjectPathImpl()).WillByDefault(::testing::Return(path1));

  EXPECT_CALL(mock_interactor, GetExistingProjectPathImpl());

  EXPECT_EQ(mock_interactor.GetExistingProjectPath(), path1);
  EXPECT_EQ(mock_interactor.GetCurrentWorkdir(), GetTestHomeDir());

  // setting mock method to return empty path as user choice
  ON_CALL(mock_interactor, GetExistingProjectPathImpl())
      .WillByDefault(::testing::Return(empty_path));
  EXPECT_CALL(mock_interactor, GetNewProjectPathImpl());

  // user interactor remembers last workdir from meaningful paths, so empty path shouldn't change
  // workdir
  EXPECT_EQ(mock_interactor.GetNewProjectPath(), empty_path);
  EXPECT_EQ(mock_interactor.GetCurrentWorkdir(), GetTestHomeDir());
}

}  // namespace mvvm
