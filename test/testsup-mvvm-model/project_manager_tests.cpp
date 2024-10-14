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
#include <mvvm/test/mock_project.h>

#include <gtest/gtest.h>

using namespace mvvm;

/**
 * @brief Tests for ProjectManager class.
 */
class ProjectManagerTests : public ::testing::Test
{
public:
  /**
   * @brief Returns a project necessary to initialize ProjectManager.
   */
  IProject* GetProject()
  {
    m_project = std::make_unique<test::ProjectDecorator>(&m_mock_project);
    return m_project.get();
  }

  mvvm::test::MockProject m_mock_project;
  std::unique_ptr<IProject> m_project;
};

TEST_F(ProjectManagerTests, AttemptToCreateProject)
{
  // callback which creates null project
  EXPECT_THROW((ProjectManager(nullptr)), RuntimeException);
}

TEST_F(ProjectManagerTests, InitialState)
{
  const ProjectManager manager(GetProject());

  EXPECT_CALL(m_mock_project, IsModified()).Times(1);
  EXPECT_CALL(m_mock_project, GetProjectPath()).Times(1);

  EXPECT_TRUE(manager.CurrentProjectPath().empty());
  EXPECT_FALSE(manager.IsModified());
}

//! Mocking project pretend it has a path defined, and it is in modified state. Checking behavior on
//! ProjectManager::SaveCurrentProject
TEST_F(ProjectManagerTests, TitledModifiedSave)
{
  ProjectManager manager(GetProject());

  // setting up what mock project should return
  const std::string path("path");
  ON_CALL(m_mock_project, IsModified()).WillByDefault(::testing::Return(true));
  ON_CALL(m_mock_project, Save(path)).WillByDefault(::testing::Return(true));
  ON_CALL(m_mock_project, GetProjectPath()).WillByDefault(::testing::Return(path));

  EXPECT_CALL(m_mock_project, GetProjectPath()).Times(1);
  EXPECT_CALL(m_mock_project, IsModified()).Times(1);

  // setting expectencies for CurrentProjectPath
  EXPECT_TRUE(manager.IsModified());
  EXPECT_EQ(manager.CurrentProjectPath(), path);

  // setting expectencies for SaveCurrentProject
  EXPECT_CALL(m_mock_project, GetProjectPath()).Times(2);  // ProjectHasPath, GetProjectPath
  EXPECT_CALL(m_mock_project, Save(path));
  EXPECT_TRUE(manager.SaveCurrentProject());
}
