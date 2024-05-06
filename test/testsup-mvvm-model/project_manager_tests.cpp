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

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/project/i_project.h>
#include <mvvm/project/project_context.h>
#include <mvvm/project/project_utils.h>
#include <mvvm/test/folder_based_test.h>
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>

using namespace mvvm;

namespace
{
const std::string samplemodel_name = "samplemodel";

}  // namespace

//! Tests for ProjectManager class.

class ProjectManagerTests : public mvvm::test::FolderBasedTest
{
public:
  ProjectManagerTests()
      : FolderBasedTest("test_ProjectManager")
      , sample_model(std::make_unique<ApplicationModel>(samplemodel_name))
  {
  }

  std::vector<SessionModelInterface*> GetModels() const { return {sample_model.get()}; };

  ProjectManager::create_project_t CreateContext()
  {
    auto result = [this]() -> std::unique_ptr<IProject>
    {
      ProjectContext context;
      context.m_models_callback = [this]() { return GetModels(); };
      context.m_modified_callback = [this]() { ++m_project_modified_count; };
      return mvvm::utils::CreateUntitledProject(context);
    };

    return result;
  }

  std::unique_ptr<ApplicationModel> sample_model;
  int m_project_modified_count{0};
};

//! Initial state of ProjectManager. Project created, and not-saved.

TEST_F(ProjectManagerTests, InitialState)
{
  ProjectManager manager(CreateContext());
  EXPECT_TRUE(manager.CurrentProjectPath().empty());
  EXPECT_FALSE(manager.IsModified());
}

// ----------------------------------------------------------------------------
// Untitled, empty project
// ----------------------------------------------------------------------------

//! Creating new project. Use untitled+empty project as a starting point.
//! Should succeed, since old empty project doesn't need to be saved.

TEST_F(ProjectManagerTests, UntitledEmptyNew)
{
  ProjectManager manager(CreateContext());

  const auto project_dir = CreateEmptyDir("Project_untitledEmptyNew");
  EXPECT_TRUE(manager.CreateNewProject(project_dir));

  EXPECT_EQ(manager.CurrentProjectPath(), project_dir);
  EXPECT_FALSE(manager.IsModified());

  // project directory should contain a json file with the model
  auto model_filename = utils::Join(project_dir, samplemodel_name + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

//! Saving of new project. Use untitled+empty project as a starting point.
//! Should fail since project directory is not defined.

TEST_F(ProjectManagerTests, UntitledEmptySave)
{
  ProjectManager manager(CreateContext());
  EXPECT_FALSE(manager.SaveCurrentProject());
  EXPECT_FALSE(manager.IsModified());
}

//! Saving of new project. Use untitled+empty project as a starting point.
//! Should be saved, file sould appear on disk.

TEST_F(ProjectManagerTests, UntitledEmptySaveAs)
{
  ProjectManager manager(CreateContext());

  const auto project_dir = CreateEmptyDir("Project_untitledEmptySaveAs");
  EXPECT_TRUE(manager.SaveProjectAs(project_dir));
  EXPECT_FALSE(manager.IsModified());

  // project directory should contain a json file with the model
  auto model_filename = utils::Join(project_dir, samplemodel_name + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

// ----------------------------------------------------------------------------
// Untitled, modified
// ----------------------------------------------------------------------------

//! Creating new project. Use untitled+modified project as a starting point.
//! Should fail, since modified old project will prevent creation of the new one.

TEST_F(ProjectManagerTests, UntitledModifiedNew)
{
  ProjectManager manager(CreateContext());

  // modifying the model
  sample_model->InsertItem<PropertyItem>();

  EXPECT_TRUE(manager.IsModified());

  const auto project_dir = CreateEmptyDir("Project_untitledModifiedNew");
  EXPECT_FALSE(manager.CreateNewProject(project_dir));

  EXPECT_TRUE(manager.CurrentProjectPath().empty());
  EXPECT_TRUE(manager.IsModified());

  // project directory should be empty
  auto model_filename = utils::Join(project_dir, samplemodel_name + ".xml");
  EXPECT_FALSE(utils::IsExists(model_filename));
}

//! Saving of new project. Use untitled+modified project as a starting point.
//! Should fail since project directory is not defined.

TEST_F(ProjectManagerTests, UntitledModifiedSave)
{
  ProjectManager manager(CreateContext());
  // modifying the model
  sample_model->InsertItem<PropertyItem>();

  EXPECT_FALSE(manager.SaveCurrentProject());
  EXPECT_TRUE(manager.IsModified());
}

//! Saving of new project. Use untitled+empty project as a starting point.
//! Should be saved, file sould appear on disk.

TEST_F(ProjectManagerTests, UntitledModifiedSaveAs)
{
  ProjectManager manager(CreateContext());
  sample_model->InsertItem<PropertyItem>();  // modifying the model

  const auto project_dir = CreateEmptyDir("Project_untitledModifiedSaveAs");
  EXPECT_TRUE(manager.SaveProjectAs(project_dir));
  EXPECT_FALSE(manager.IsModified());

  // project directory should contain a json file with the model
  auto model_filename = utils::Join(project_dir, samplemodel_name + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

// ----------------------------------------------------------------------------
// Titled, unmodified
// ----------------------------------------------------------------------------

//! Creating new project. Use titled+unmodified project as a starting point.
//! Should succeed, since old empty project doesn't need to be saved.

TEST_F(ProjectManagerTests, TitledUnmodifiedNew)
{
  ProjectManager manager(CreateContext());

  const auto project_dir = CreateEmptyDir("Project_titledUnmodifiedNew");
  EXPECT_TRUE(manager.SaveProjectAs(project_dir));
  EXPECT_EQ(manager.CurrentProjectPath(), project_dir);

  const auto project_dir2 = CreateEmptyDir("Project_titledUnmodifiedNew2");
  EXPECT_TRUE(manager.CreateNewProject(project_dir2));

  EXPECT_EQ(manager.CurrentProjectPath(), project_dir2);
  EXPECT_FALSE(manager.IsModified());

  // project directory should contain a json file with the model
  auto model_filename = utils::Join(project_dir2, samplemodel_name + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

// ----------------------------------------------------------------------------
// Titled, modified
// ----------------------------------------------------------------------------

//! Saving of new project. Use titled+modified project as a starting point.
//! Should succeed.

TEST_F(ProjectManagerTests, TitledModifiedSave)
{
  ProjectManager manager(CreateContext());

  const auto project_dir = CreateEmptyDir("Project_titledModifiedSave");
  EXPECT_TRUE(manager.SaveProjectAs(project_dir));
  EXPECT_EQ(manager.CurrentProjectPath(), project_dir);

  // modifying the model
  sample_model->InsertItem<PropertyItem>();

  EXPECT_TRUE(manager.SaveCurrentProject());
  EXPECT_FALSE(manager.IsModified());
}

// ----------------------------------------------------------------------------
// Callbacks
// ----------------------------------------------------------------------------

TEST_F(ProjectManagerTests, Callback)
{
  auto context = CreateContext();

  ProjectManager manager(context);

  EXPECT_EQ(m_project_modified_count, 0);

  // saving the project
  const auto project_dir = CreateEmptyDir("Project_callback");
  EXPECT_TRUE(manager.SaveProjectAs(project_dir));
  EXPECT_EQ(manager.CurrentProjectPath(), project_dir);
  EXPECT_EQ(m_project_modified_count, 0);

  // modifying the model
  sample_model->InsertItem<PropertyItem>();
  EXPECT_EQ(m_project_modified_count, 1);
  EXPECT_TRUE(manager.IsModified());

  // modifying the model second time
  sample_model->InsertItem<PropertyItem>();
  EXPECT_EQ(m_project_modified_count, 1);  // do not sum up
  EXPECT_TRUE(manager.IsModified());

  EXPECT_TRUE(manager.SaveCurrentProject());
  EXPECT_FALSE(manager.IsModified());
  EXPECT_EQ(m_project_modified_count, 1);
}
