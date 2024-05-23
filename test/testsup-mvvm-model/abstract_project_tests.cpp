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

#include "mvvm/project/abstract_project.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>

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

    MOCK_METHOD(bool, SaveImpl, (const std::string&), (override));
    MOCK_METHOD(bool, LoadImpl, (const std::string&), (override));
  };

  ProjectContext CreateContext(const std::string& application_type = {})
  {
    ProjectContext result;
    result.modified_callback = m_callback.AsStdFunction();
    result.models_callback = [this]() { return std::vector<SessionModelInterface*>({&m_model}); };
    result.application_type = application_type;
    return result;
  }

  ApplicationModel m_model;
  ::testing::MockFunction<void(void)> m_callback;
};

TEST_F(AbstractProjectTest, InitialState)
{
  const std::string expected_app_type("MyApp");
  MockAbstractProject project(ProjectType::kFolderBased, CreateContext(expected_app_type));
  EXPECT_EQ(project.GetProjectType(), ProjectType::kFolderBased);
  EXPECT_EQ(project.GetApplicationType(), expected_app_type);
  EXPECT_FALSE(project.IsModified());
  EXPECT_TRUE(project.GetProjectPath().empty());
}

//! The project should automatically clean models.

TEST_F(AbstractProjectTest, ModelCleanup)
{
  m_model.InsertItem<PropertyItem>();
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 1);

  const std::string expected_app_type("MyApp");
  MockAbstractProject project(ProjectType::kFileBased, CreateContext(expected_app_type));

  EXPECT_FALSE(project.IsModified());
  EXPECT_EQ(m_model.GetRootItem()->GetTotalItemCount(), 0);
}

//! Testing successfull save. Correct methods should be called, the project should get correct path.
TEST_F(AbstractProjectTest, SuccessfullSave)
{
  const std::string expected_path("path");
  MockAbstractProject project(ProjectType::kFolderBased, CreateContext());

  EXPECT_TRUE(project.GetProjectPath().empty());
  EXPECT_FALSE(project.IsModified());

  // modifying model
  EXPECT_CALL(m_callback, Call()).Times(1);

  m_model.InsertItem<PropertyItem>();
  EXPECT_TRUE(project.IsModified());

  // mimicking successfull save
  ON_CALL(project, SaveImpl(expected_path)).WillByDefault(::testing::Return(true));

  // setting expectations
  EXPECT_CALL(project, SaveImpl(expected_path)).Times(1);

  // performing save and triggering expectations
  EXPECT_TRUE(project.Save(expected_path));

  EXPECT_FALSE(project.IsModified());
  EXPECT_EQ(project.GetProjectPath(), expected_path);
}

//! Testing failed save. Correct methods should be called, the project should stay without path.
TEST_F(AbstractProjectTest, FailedSave)
{
  const std::string expected_path("path");
  MockAbstractProject project(ProjectType::kFolderBased, CreateContext());

  EXPECT_TRUE(project.GetProjectPath().empty());

  // mimicking failed save
  ON_CALL(project, SaveImpl(expected_path)).WillByDefault(::testing::Return(false));

  // setting expectations
  EXPECT_CALL(project, SaveImpl(expected_path)).Times(1);

  // performing save and triggering expectations
  EXPECT_FALSE(project.Save(expected_path));

  EXPECT_FALSE(project.IsModified());

  // path remained empty
  EXPECT_TRUE(project.GetProjectPath().empty());
}

//! Testing successfull load. Correct methods should be called, the project should get correct path.
TEST_F(AbstractProjectTest, SuccessfullLoad)
{
  const std::string expected_path("path");
  MockAbstractProject project(ProjectType::kFolderBased, CreateContext());

  EXPECT_TRUE(project.GetProjectPath().empty());

  // mimicking successfull load
  ON_CALL(project, LoadImpl(expected_path)).WillByDefault(::testing::Return(true));

  // setting expectations
  EXPECT_CALL(project, LoadImpl(expected_path)).Times(1);

  // performing save and triggering expectations
  EXPECT_TRUE(project.Load(expected_path));

  EXPECT_FALSE(project.IsModified());
  EXPECT_EQ(project.GetProjectPath(), expected_path);
}

//! Testing successfull load. Correct methods should be called, the project should stay without
//! path.
TEST_F(AbstractProjectTest, FailedLoad)
{
  const std::string expected_path("path");
  MockAbstractProject project(ProjectType::kFolderBased, CreateContext());

  EXPECT_TRUE(project.GetProjectPath().empty());

  // mimicking failed load
  ON_CALL(project, LoadImpl(expected_path)).WillByDefault(::testing::Return(false));

  // setting expectations
  EXPECT_CALL(project, LoadImpl(expected_path)).Times(1);

  // performing save and triggering expectations
  EXPECT_FALSE(project.Load(expected_path));

  EXPECT_FALSE(project.IsModified());

  // path remained empty
  EXPECT_TRUE(project.GetProjectPath().empty());
}
