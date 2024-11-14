/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
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

#include "mvvm/project/project_manager.h"

#include <mvvm/core/exceptions.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>
#include <testutils/mock_project.h>
#include <testutils/mock_user_interactor.h>

using namespace mvvm;
using ::testing::_;

/**
 * @brief Tests for ProjectManagerDecorator class.
 *
 * In this test we are using mock project and mock user interactor, no actual saving on disk is
 * performed.
 */
class ProjectManagerTest : public ::testing::Test
{
public:
  /**
   * @brief Creates user context with callbacks mimicking user responce.
   *
   * @param new_path The user reply to the request to select a new document path.
   * @param existing_path The user reply to the request to select the existing document path.
   * @param answer The user reply to the question if changes should be saved.
   */
  UserInteractionContext CreateUserContext(const std::string& new_path = {},
                                           const std::string& existing_path = {},
                                           SaveChangesAnswer answer = SaveChangesAnswer::kSave)
  {
    ON_CALL(m_mock_interactor, GetExistingProjectPath())
        .WillByDefault(::testing::Return(existing_path));
    ON_CALL(m_mock_interactor, OnGetNewProjectPath()).WillByDefault(::testing::Return(new_path));
    ON_CALL(m_mock_interactor, OnSaveChangesRequest()).WillByDefault(::testing::Return(answer));
    return m_mock_interactor.CreateContext();
  }

  test::MockUserInteractor m_mock_interactor;
  mvvm::test::MockProject m_mock_project;
};

TEST_F(ProjectManagerTest, InitialState)
{
  const ProjectManager decorator(&m_mock_project, CreateUserContext("", ""));

  EXPECT_CALL(m_mock_project, IsModified()).Times(1);

  EXPECT_FALSE(decorator.GetProject()->HasPath());
  EXPECT_FALSE(decorator.IsModified());
}

//! Testing scenario when project implementation throws an exception during project load.
TEST_F(ProjectManagerTest, ExceptionDuringProjectLoad)
{
  const std::string file_name = "file.xml";

  ProjectManager decorator(&m_mock_project, CreateUserContext("", file_name));

  // setting up the project
  ON_CALL(m_mock_project, IsModified()).WillByDefault(::testing::Return(false));
  ON_CALL(m_mock_project, Load(_)).WillByDefault(::testing::Throw(RuntimeException("Failed load")));

  // one IsModified() from ProjectManager, another from SaveBeforeClosing
  EXPECT_CALL(m_mock_project, IsModified()).Times(1);
  EXPECT_CALL(m_mock_project, Load(file_name)).Times(1);
  // we shouldn't close previous project if loading of the new one is failed
  EXPECT_CALL(m_mock_project, Close()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnMessage(_)).Times(1);  // report about exception

  EXPECT_FALSE(decorator.OpenExistingProject(file_name));
}

//! Mocking project pretends it has a path defined, and it is in modified state. Checking behavior
//! on ProjectManager::SaveCurrentProject
TEST_F(ProjectManagerTest, TitledModifiedSave)
{
  ProjectManager decorator(&m_mock_project, CreateUserContext("", ""));

  // setting up what mock project should return
  const std::string path("path");
  ON_CALL(m_mock_project, IsModified()).WillByDefault(::testing::Return(true));
  ON_CALL(m_mock_project, Save(path)).WillByDefault(::testing::Return(true));
  ON_CALL(m_mock_project, GetPath()).WillByDefault(::testing::Return(path));

  EXPECT_CALL(m_mock_project, GetPath()).Times(1);
  EXPECT_CALL(m_mock_project, IsModified()).Times(1);

  // setting expectencies for CurrentProjectPath
  EXPECT_TRUE(decorator.IsModified());
  EXPECT_EQ(decorator.GetProject()->GetPath(), path);

  // setting expectencies for SaveCurrentProject
  EXPECT_CALL(m_mock_project, GetPath()).Times(1);
  EXPECT_CALL(m_mock_project, Save(path));
  EXPECT_TRUE(decorator.SaveCurrentProject());
}

//! Mocking project pretends it has a path defined, and it is in modified state. Checking the
//! behavior of ProjectManager::OpenExistingProject when user decides to discard previous changes.
TEST_F(ProjectManagerTest, TitledModifiedDiscardAndOpenExisting)
{
  const std::string existing_project_path("existing_project_path.xml");

  ProjectManager decorator(
      &m_mock_project, CreateUserContext("", existing_project_path, SaveChangesAnswer::kDiscard));

  // setting up what mock project should return
  ON_CALL(m_mock_project, IsModified()).WillByDefault(::testing::Return(true));
  ON_CALL(m_mock_project, Load(existing_project_path)).WillByDefault(::testing::Return(true));

  {  // expectations
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_project, IsModified()).Times(1);
    EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(1);
    EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(0);
    EXPECT_CALL(m_mock_project, Load(existing_project_path)).Times(1);
  }

  // triggering action, the path to existing project is known
  EXPECT_TRUE(decorator.OpenExistingProject(existing_project_path));
}

//! Mocking project pretends it has a path defined, and it is in modified state. Checking the
//! behavior of ProjectManager::OpenExistingProject when user decides to discard previous changes.
//! Same as previous test, with only difference that no name is provided while opening existing
//! project. This validates the call to "select existing project" dialog.
TEST_F(ProjectManagerTest, TitledModifiedDiscardAndOpenExistingV2)
{
  const std::string existing_project_path("existing_project_path.xml");

  ProjectManager decorator(
      &m_mock_project, CreateUserContext("", existing_project_path, SaveChangesAnswer::kDiscard));

  // setting up what mock project should return
  ON_CALL(m_mock_project, IsModified()).WillByDefault(::testing::Return(true));
  ON_CALL(m_mock_project, Load(existing_project_path)).WillByDefault(::testing::Return(true));

  {  // expectations
    const ::testing::InSequence seq;
    EXPECT_CALL(m_mock_project, IsModified()).Times(1);
    EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(1);
    EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(1);
    EXPECT_CALL(m_mock_project, Load(existing_project_path)).Times(1);
  }

  // triggering action, the path to existing project is unknown
  EXPECT_TRUE(decorator.OpenExistingProject(""));
}
