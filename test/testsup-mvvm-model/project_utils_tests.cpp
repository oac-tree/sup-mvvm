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

#include "mvvm/project/project_utils.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/project/i_project.h>
#include <mvvm/project/project_context.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

using namespace mvvm;

//! Tests of ProjectUtils namespace functions.

class ProjectUtilsTest : public mvvm::test::FolderTest
{
public:
  ProjectUtilsTest()
      : FolderTest("ProjectUtilsTest")
      , m_sample_model(std::make_unique<ApplicationModel>("SampleModel"))
  {
  }

  std::vector<ISessionModel*> GetModels() const { return {m_sample_model.get()}; };

  ProjectContext CreateContext() { return {}; }

  std::unique_ptr<ApplicationModel> m_sample_model;
};

//! Testing helper structure.

TEST_F(ProjectUtilsTest, SuggestFileName)
{
  ApplicationModel model("TestModel");
  EXPECT_EQ(std::string("testmodel.xml"), utils::SuggestFileName(model));
}

TEST_F(ProjectUtilsTest, CreateUntitledProject)
{
  auto project =
      utils::CreateUntitledProject(ProjectType::kFileBased, GetModels(), CreateContext());
  EXPECT_TRUE(project->GetProjectPath().empty());
}

TEST_F(ProjectUtilsTest, ProjectWindowTitle)
{
  // untitled and unmodified project
  EXPECT_EQ(utils::ProjectWindowTitle(std::string(""), false), "Untitled");

  // untitled and modified project
  EXPECT_EQ(utils::ProjectWindowTitle(std::string(""), true), "*Untitled");

  // unmodified project without projectDir
  EXPECT_EQ(utils::ProjectWindowTitle(std::string("Untitled"), false), "Untitled");

  // modified project without projectDir
  EXPECT_EQ(utils::ProjectWindowTitle(std::string("Untitled"), true), "*Untitled");

  // unmodified project with projectDir
  EXPECT_EQ(utils::ProjectWindowTitle(std::string("/home/user/project1"), false), "project1");

  // modified project with projectDir
  EXPECT_EQ(utils::ProjectWindowTitle(std::string("/home/user/project1"), true), "*project1");
}

TEST_F(ProjectUtilsTest, ProjectWindowTitleViaProjectInterface)
{
  auto project =
      utils::CreateUntitledProject(ProjectType::kFolderBased, GetModels(), CreateContext());

  // unmodified project without projectDir
  EXPECT_EQ(utils::ProjectWindowTitle(*project), "Untitled");

  m_sample_model->InsertItem<PropertyItem>();
  EXPECT_EQ(utils::ProjectWindowTitle(*project), "*Untitled");

  // saving in a project directory
  project->Save(GetTestHomeDir());
  EXPECT_EQ(utils::ProjectWindowTitle(*project), GetTestHomeDirName());

  // modifying
  m_sample_model->InsertItem<PropertyItem>();
  EXPECT_EQ(utils::ProjectWindowTitle(*project), "*" + GetTestHomeDirName());
}

TEST_F(ProjectUtilsTest, IsPossibleProjectDir)
{
  auto project =
      utils::CreateUntitledProject(ProjectType::kFolderBased, GetModels(), CreateContext());

  // empty directory can't be a project directory
  auto dirname = CreateEmptyDir("test_IsPossibleProjectDir");
  EXPECT_FALSE(utils::IsPossibleProjectDir(dirname));

  project->Save(dirname);
  EXPECT_TRUE(utils::IsPossibleProjectDir(dirname));
}
