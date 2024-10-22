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

#include "mvvm/project/project_manager_decorator.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/project/project_manager.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>
#include <testutils/mock_project.h>
#include <testutils/mock_user_interactor.h>

using namespace mvvm;
using ::testing::_;
/**
 * @brief Tests for ProjectManagerDecorator class.
 */
class ProjectManagerDecoratorTest : public ::testing::Test
{
public:
  /**
   * @brief Creates user context with callbacks mimicking user responce.
   */
  UserInteractionContext CreateUserContext(const std::string& new_path = {},
                                           const std::string& existing_path = {})
  {
    ON_CALL(m_mock_interactor, GetExistingProjectPath())
        .WillByDefault(::testing::Return(existing_path));
    ON_CALL(m_mock_interactor, OnGetNewProjectPath()).WillByDefault(::testing::Return(new_path));
    return m_mock_interactor.CreateContext();
  }

  test::MockUserInteractor m_mock_interactor;
  mvvm::test::MockProject m_mock_project;
};

//! Testing scenario when project implementation throws an exception during project load.
TEST_F(ProjectManagerDecoratorTest, ExceptionDuringProjectLoad)
{
  const std::string file_name = "file.xml";

  auto manager = std::make_unique<ProjectManager>(&m_mock_project);
  ProjectManagerDecorator decorator(std::move(manager), CreateUserContext("", file_name));

  // setting up the project
  ON_CALL(m_mock_project, IsModified()).WillByDefault(::testing::Return(false));
  ON_CALL(m_mock_project, Load(_)).WillByDefault(::testing::Throw(RuntimeException("Failed load")));

  // one IsModified() from ProjectManager, another from SaveBeforeClosing
  EXPECT_CALL(m_mock_project, IsModified()).Times(2);
  EXPECT_CALL(m_mock_project, Load(file_name)).Times(1);
  // we shouldn't close previous project if loading of the new one is failed
  EXPECT_CALL(m_mock_project, CloseProject()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnMessage(_)).Times(1);  // report about exception

  EXPECT_FALSE(decorator.OpenExistingProject(file_name));
}
