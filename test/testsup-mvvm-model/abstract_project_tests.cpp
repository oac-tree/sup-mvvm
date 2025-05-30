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

#include "mvvm/project/abstract_project.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/test/mock_project_context.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for AbstractProject class.

class AbstractProjectTest : public ::testing::Test
{
public:
  class MockAbstractProject : public mvvm::AbstractProject
  {
  public:
    MockAbstractProject(ProjectType project_type, ProjectContext context)
        : mvvm::AbstractProject(project_type, context)
    {
    }

    MockAbstractProject(ProjectType project_type, const std::vector<ISessionModel*>& models,
                        ProjectContext context)
        : mvvm::AbstractProject(project_type, context), m_models(models)
    {
    }

    MOCK_METHOD(bool, SaveImpl, (const std::string&), (override));
    MOCK_METHOD(bool, LoadImpl, (const std::string&), (override));
    MOCK_METHOD(bool, CloseProjectImpl, (), (override));
    MOCK_METHOD(bool, CreateEmptyProjectImpl, (), (override));

    std::vector<ISessionModel*> GetModels() const override { return m_models; }

    std::vector<ISessionModel*> m_models;
  };

  ProjectContext CreateContext(const std::string& application_type = {})
  {
    return m_mock_project_context.CreateContext(application_type);
  }

  ApplicationModel m_model;
  test::MockProjectContext m_mock_project_context;
};

TEST_F(AbstractProjectTest, InitialState)
{
  const std::string expected_app_type("MyApp");
  MockAbstractProject project(ProjectType::kFolderBased, CreateContext(expected_app_type));
  EXPECT_EQ(project.GetProjectType(), ProjectType::kFolderBased);
  EXPECT_EQ(project.GetApplicationType(), expected_app_type);
  EXPECT_FALSE(project.IsModified());
  EXPECT_TRUE(project.GetPath().empty());
}

//! By default AbstractProject doesn't clear models.
TEST_F(AbstractProjectTest, ModelCleanup)
{
  m_model.InsertItem<PropertyItem>();
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);

  const std::string expected_app_type("MyApp");
  MockAbstractProject project(ProjectType::kFileBased, CreateContext(expected_app_type));

  EXPECT_FALSE(project.IsModified());
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);
}

//! Testing successfull save. Correct methods should be called, the project should get correct path.
TEST_F(AbstractProjectTest, SuccessfullSave)
{
  const std::string expected_path("path");

  MockAbstractProject project(ProjectType::kFolderBased, {&m_model}, CreateContext());

  // mimicking successfull save
  ON_CALL(project, SaveImpl(expected_path)).WillByDefault(::testing::Return(true));
  // mimicking successfull new project creation
  ON_CALL(project, CreateEmptyProjectImpl()).WillByDefault(::testing::Return(true));

  // setting expectations
  EXPECT_CALL(m_mock_project_context, OnLoaded()).Times(1);
  EXPECT_CALL(project, CreateEmptyProjectImpl()).Times(1);

  EXPECT_TRUE(project.CreateEmpty());

  EXPECT_TRUE(project.GetPath().empty());
  EXPECT_FALSE(project.IsModified());

  // modifying model
  EXPECT_CALL(m_mock_project_context, OnModified()).Times(1);
  m_model.InsertItem<PropertyItem>();
  EXPECT_TRUE(project.IsModified());

  // setting expectations
  EXPECT_CALL(project, SaveImpl(expected_path)).Times(1);
  EXPECT_CALL(m_mock_project_context, OnSaved()).Times(1);

  // performing save and triggering expectations
  EXPECT_TRUE(project.Save(expected_path));

  EXPECT_FALSE(project.IsModified());
  EXPECT_EQ(project.GetPath(), expected_path);
}

//! Testing CreateEmpty() method. The project should loose its path.
TEST_F(AbstractProjectTest, CreateNewProject)
{
  const std::string expected_path("path");
  MockAbstractProject project(ProjectType::kFolderBased, CreateContext());

  // mimicking successfull save
  ON_CALL(project, CreateEmptyProjectImpl()).WillByDefault(::testing::Return(true));
  ON_CALL(project, SaveImpl(expected_path)).WillByDefault(::testing::Return(true));

  // setting up expectations
  EXPECT_CALL(m_mock_project_context, OnLoaded()).Times(1);
  EXPECT_CALL(project, CreateEmptyProjectImpl()).Times(1);
  EXPECT_CALL(m_mock_project_context, OnSaved()).Times(1);

  // setting expectations
  EXPECT_CALL(project, SaveImpl(expected_path)).Times(1);

  // performing save and triggering expectations
  EXPECT_TRUE(project.Save(expected_path));

  EXPECT_FALSE(project.IsModified());
  EXPECT_EQ(project.GetPath(), expected_path);

  EXPECT_TRUE(project.CreateEmpty());
  EXPECT_TRUE(project.GetPath().empty());
}

//! Testing Close() method. The project should loose its path.
TEST_F(AbstractProjectTest, CloseProject)
{
  const std::string expected_path("path");
  MockAbstractProject project(ProjectType::kFolderBased, CreateContext());

  // mimicking successfull save
  ON_CALL(project, SaveImpl(expected_path)).WillByDefault(::testing::Return(true));
  ON_CALL(project, CloseProjectImpl()).WillByDefault(::testing::Return(true));

  // setting expectations
  EXPECT_CALL(project, SaveImpl(expected_path)).Times(1);
  EXPECT_CALL(project, CloseProjectImpl()).Times(1);
  EXPECT_CALL(m_mock_project_context, OnSaved()).Times(1);

  // performing save and triggering expectations
  EXPECT_TRUE(project.Save(expected_path));

  EXPECT_FALSE(project.IsModified());
  EXPECT_EQ(project.GetPath(), expected_path);

  EXPECT_TRUE(project.Close());
  EXPECT_TRUE(project.GetPath().empty());
}

//! Testing failed save. Correct methods should be called, the project should stay without path.
TEST_F(AbstractProjectTest, FailedSave)
{
  const std::string expected_path("path");
  MockAbstractProject project(ProjectType::kFolderBased, CreateContext());

  EXPECT_TRUE(project.GetPath().empty());

  // mimicking failed save
  ON_CALL(project, SaveImpl(expected_path)).WillByDefault(::testing::Return(false));

  // setting expectations
  EXPECT_CALL(project, SaveImpl(expected_path)).Times(1);

  // performing save and triggering expectations
  EXPECT_FALSE(project.Save(expected_path));

  EXPECT_FALSE(project.IsModified());

  // path remained empty
  EXPECT_TRUE(project.GetPath().empty());
}

//! Testing successfull load. Correct methods should be called, the project should get correct path.
TEST_F(AbstractProjectTest, SuccessfullLoad)
{
  const std::string expected_path("path");
  MockAbstractProject project(ProjectType::kFolderBased, CreateContext());

  EXPECT_TRUE(project.GetPath().empty());

  // mimicking successfull load
  ON_CALL(project, LoadImpl(expected_path)).WillByDefault(::testing::Return(true));

  // setting expectations
  EXPECT_CALL(project, LoadImpl(expected_path)).Times(1);
  EXPECT_CALL(m_mock_project_context, OnLoaded()).Times(1);

  // performing save and triggering expectations
  EXPECT_TRUE(project.Load(expected_path));

  EXPECT_FALSE(project.IsModified());
  EXPECT_EQ(project.GetPath(), expected_path);
}

//! Testing successfull load. Correct methods should be called, the project should stay without
//! path.
TEST_F(AbstractProjectTest, FailedLoad)
{
  const std::string expected_path("path");
  MockAbstractProject project(ProjectType::kFolderBased, CreateContext());

  EXPECT_TRUE(project.GetPath().empty());

  // mimicking failed load
  ON_CALL(project, LoadImpl(expected_path)).WillByDefault(::testing::Return(false));

  // setting expectations
  EXPECT_CALL(project, LoadImpl(expected_path)).Times(1);
  EXPECT_CALL(m_mock_project_context, OnLoaded()).Times(0);

  // performing save and triggering expectations
  EXPECT_FALSE(project.Load(expected_path));

  EXPECT_FALSE(project.IsModified());

  // path remained empty
  EXPECT_TRUE(project.GetPath().empty());
}
