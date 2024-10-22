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

#include "mvvm/project/app_project.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/utils/container_utils.h>
#include <mvvm/utils/file_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/folder_test.h>

using namespace mvvm;

namespace
{
const std::string kApplicationType("TestApplication");
const std::string ModelTypeA = "ModelTypeA";
const std::string ModelTypeB = "ModelTypeB";
}  // namespace

/**
 * @brief Tests for AppProject class.
 */
class AppProjectTest : public mvvm::test::FolderTest
{
public:
  AppProjectTest() : FolderTest("test_AppProjectTest") {}

  std::unique_ptr<AppProject> CreateProject()
  {
    mvvm::ProjectContext context(
        {m_modified_callback.AsStdFunction(), m_loaded_callback.AsStdFunction(), kApplicationType});
    return std::make_unique<AppProject>(context);
  }

  class TestModelA : public mvvm::ApplicationModel
  {
  public:
    TestModelA() : ApplicationModel(ModelTypeA) {}
  };

  class TestModelB : public mvvm::ApplicationModel
  {
  public:
    TestModelB() : ApplicationModel(ModelTypeB) {}
  };

  ::testing::MockFunction<void(void)> m_modified_callback;
  ::testing::MockFunction<void(void)> m_loaded_callback;
};

TEST_F(AppProjectTest, InitialState)
{
  auto project = CreateProject();

  EXPECT_TRUE(project->GetProjectPath().empty());
  EXPECT_EQ(project->GetProjectType(), mvvm::ProjectType::kFileBased);
  EXPECT_EQ(project->GetApplicationType(), kApplicationType);
  EXPECT_EQ(project->GetModelCount(), 0);

  EXPECT_TRUE(project->GetModels().empty());
  EXPECT_FALSE(project->IsModified());

  // attempt to create project when no models are registered
  EXPECT_FALSE(project->CreateNewProject());
}

TEST_F(AppProjectTest, RegisterModelsAndCreateProject)
{
  auto project = CreateProject();

  EXPECT_TRUE(project->GetProjectPath().empty());
  EXPECT_EQ(project->GetProjectType(), mvvm::ProjectType::kFileBased);
  EXPECT_EQ(project->GetApplicationType(), kApplicationType);

  project->RegisterModel<TestModelA>();
  project->RegisterModel<TestModelB>();

  EXPECT_TRUE(project->GetModels().empty());
  EXPECT_FALSE(project->IsModified());
  EXPECT_EQ(project->GetModelCount(), 0);

  // setting up expectations before project creation
  EXPECT_CALL(m_loaded_callback, Call()).Times(1);

  EXPECT_TRUE(project->CreateNewProject());

  ASSERT_EQ(project->GetModelCount(), 2);
  ASSERT_NE(dynamic_cast<TestModelA*>(project->GetModels().at(0)), nullptr);
  ASSERT_NE(dynamic_cast<TestModelB*>(project->GetModels().at(1)), nullptr);
}

TEST_F(AppProjectTest, CreateNewProjectThenModifyThenClose)
{
  auto project = CreateProject();

  EXPECT_EQ(project->RegisterModel<TestModelA>(), 0);
  EXPECT_EQ(project->RegisterModel<TestModelB>(), 1);

  // setting up expectations before project creation
  EXPECT_CALL(m_loaded_callback, Call()).Times(1);

  EXPECT_TRUE(project->CreateNewProject());

  EXPECT_TRUE(project->GetProjectPath().empty());
  ASSERT_EQ(project->GetModelCount(), 2);
  EXPECT_FALSE(project->IsModified());

  ASSERT_NE(project->GetModel<TestModelA>(0U), nullptr);
  ASSERT_NE(project->GetModel<TestModelB>(1U), nullptr);

  EXPECT_THROW(project->GetModel<TestModelB>(0U), RuntimeException);  // wront type
  EXPECT_THROW(project->GetModel<TestModelB>(2U), RuntimeException);  // wrong index

  // setting up expectation before project modification
  EXPECT_CALL(m_modified_callback, Call()).Times(1);

  // modifying a project
  project->GetModel<TestModelB>(1U)->InsertItem<mvvm::SessionItem>();
  EXPECT_TRUE(project->IsModified());

  // closing project
  EXPECT_TRUE(project->CloseProject());
  EXPECT_FALSE(project->IsModified());
  ASSERT_EQ(project->GetModelCount(), 0);
}

TEST_F(AppProjectTest, SaveAndClose)
{
  const auto expected_path = GetFilePath("untitled.xml");

  auto project = CreateProject();
  project->RegisterModel<TestModelA>();
  project->RegisterModel<TestModelB>();

  // setting up expectations before project creation
  EXPECT_CALL(m_loaded_callback, Call()).Times(1);

  EXPECT_TRUE(project->CreateNewProject());

  // setting up expectation before project modification
  EXPECT_CALL(m_modified_callback, Call()).Times(1);

  project->GetModel<TestModelB>(1U)->InsertItem<mvvm::SessionItem>();
  EXPECT_TRUE(project->IsModified());

  EXPECT_TRUE(project->Save(expected_path));

  EXPECT_EQ(project->GetProjectPath(), expected_path);
  EXPECT_FALSE(project->IsModified());

  // closing project
  EXPECT_TRUE(project->CloseProject());
  EXPECT_FALSE(project->IsModified());
  ASSERT_EQ(project->GetModelCount(), 0);

  EXPECT_TRUE(mvvm::utils::IsExists(expected_path));
}

TEST_F(AppProjectTest, SaveAndLoad)
{
  const auto expected_path = GetFilePath("untitled2.xml");

  auto project = CreateProject();
  project->RegisterModel<TestModelA>();
  project->RegisterModel<TestModelB>();

  // setting up expectations before project creation
  EXPECT_CALL(m_loaded_callback, Call()).Times(1);

  EXPECT_TRUE(project->CreateNewProject());

  // setting up expectation before project modification
  EXPECT_CALL(m_modified_callback, Call()).Times(1);

  auto item = project->GetModel<TestModelB>(1U)->InsertItem<mvvm::SessionItem>();
  item->SetData(42);
  EXPECT_TRUE(project->IsModified());

  EXPECT_TRUE(project->Save(expected_path));

  // setting up expectation before project modification
  EXPECT_CALL(m_modified_callback, Call()).Times(1);

  // modifying further after save was made
  item->SetData(43);

  // setting up expectation before document load
  EXPECT_CALL(m_loaded_callback, Call()).Times(1);

  EXPECT_TRUE(project->Load(expected_path));
  EXPECT_FALSE(project->IsModified());

  // checking content of the model
  auto recreated_model = project->GetModel<TestModelB>(1U);
  ASSERT_NE(recreated_model, nullptr);
  ASSERT_NE(recreated_model->GetRootItem()->GetItem(mvvm::TagIndex::First()), nullptr);
  // expecting old value which we had at the moment of save
  EXPECT_EQ(recreated_model->GetRootItem()->GetItem(mvvm::TagIndex::First())->Data<int>(), 42);

  // closing, and loading again
  EXPECT_TRUE(project->CloseProject());

  // setting up expectation before document load
  EXPECT_CALL(m_loaded_callback, Call()).Times(1);

  EXPECT_TRUE(project->Load(expected_path));
  recreated_model = project->GetModel<TestModelB>(1U);
  ASSERT_NE(recreated_model, nullptr);
  ASSERT_NE(recreated_model->GetRootItem()->GetItem(mvvm::TagIndex::First()), nullptr);
  // expecting old value which we had at the moment of save
  EXPECT_EQ(recreated_model->GetRootItem()->GetItem(mvvm::TagIndex::First())->Data<int>(), 42);
}

//! Loading the model from wong file.
TEST_F(AppProjectTest, LoadFromWrongFile)
{
  const auto expected_path = GetFilePath("TestModelA.xml");

  auto project = CreateProject();
  project->RegisterModel<TestModelA>();

  // attempt to save without project creation
  EXPECT_THROW(project->Save(expected_path), RuntimeException);

  EXPECT_TRUE(project->CreateNewProject());

  EXPECT_TRUE(project->Save(expected_path));

  // creating project based on another model and trying to load file from another model
  auto project2 = CreateProject();
  project2->RegisterModel<TestModelB>();
  EXPECT_THROW(project2->Load(expected_path), RuntimeException);
}
