/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

#include <gtest/gtest.h>
#include <mvvm/interfaces/project_interface.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/project/project_types.h>
#include <testutils/folder_based_test.h>

using namespace mvvm;

//! Tests of ProjectUtils namespace functions.

class ProjectUtilsTests : public testutils::FolderBasedTest
{
public:
  ProjectUtilsTests()
      : FolderBasedTest("test_ProjectUtils")
      , sample_model(std::make_unique<ApplicationModel>("SampleModel"))
  {
  }

  std::vector<SessionModelInterface*> models() const { return {sample_model.get()}; };

  ProjectContext createContext()
  {
    ProjectContext result;
    result.m_models_callback = [this]() { return models(); };
    return result;
  }

  std::unique_ptr<ApplicationModel> sample_model;
};

//! Testing helper structure.

TEST_F(ProjectUtilsTests, SuggestFileName)
{
  ApplicationModel model("TestModel");
  EXPECT_EQ(std::string("testmodel.xml"), ProjectUtils::SuggestFileName(model));
}

TEST_F(ProjectUtilsTests, CreateUntitledProject)
{
  auto project = ProjectUtils::CreateUntitledProject(createContext());
  EXPECT_TRUE(project->GetProjectDir().empty());
}

TEST_F(ProjectUtilsTests, ProjectWindowTitle)
{
  auto project = ProjectUtils::CreateUntitledProject(createContext());

  // unmodified project without projectDir
  EXPECT_EQ(ProjectUtils::ProjectWindowTitle(*project), "Untitled");

  sample_model->InsertItem<PropertyItem>();
  EXPECT_EQ(ProjectUtils::ProjectWindowTitle(*project), "*Untitled");

  // saving in a project directory
  project->Save(GetTestHomeDir());
  EXPECT_EQ(ProjectUtils::ProjectWindowTitle(*project), GetTestHomeDirName());

  // modifying
  sample_model->InsertItem<PropertyItem>();
  EXPECT_EQ(ProjectUtils::ProjectWindowTitle(*project), "*" + GetTestHomeDirName());
}

TEST_F(ProjectUtilsTests, IsPossibleProjectDir)
{
  auto project = ProjectUtils::CreateUntitledProject(createContext());

  // empty directory can't be a project directory
  auto dirname = CreateEmptyDir("test_IsPossibleProjectDir");
  EXPECT_FALSE(ProjectUtils::IsPossibleProjectDir(dirname));

  project->Save(dirname);
  EXPECT_TRUE(ProjectUtils::IsPossibleProjectDir(dirname));
}
