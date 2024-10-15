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
#include <mvvm/test/folder_test.h>
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>

using namespace mvvm;

namespace
{
const std::string kSampleModelName = "samplemodel";

}  // namespace

/**
 * @brief Tests for ProjectManager class for folder-based and file-based documents.
 */
class ProjectManagerFolderTests : public mvvm::test::FolderTest
{
public:
  ProjectManagerFolderTests()
      : FolderTest("test_ProjectManager")
      , sample_model(std::make_unique<ApplicationModel>(kSampleModelName))
  {
  }

  std::vector<ISessionModel*> GetModels() const { return {sample_model.get()}; };

  IProject* GetProject(ProjectType project_type)
  {
    ProjectContext context;
    context.models = GetModels();
    context.modified_callback = [this]() { ++m_project_modified_count; };
    m_project = mvvm::utils::CreateUntitledProject(project_type, context);
    return m_project.get();
  }

  std::unique_ptr<ApplicationModel> sample_model;
  std::unique_ptr<IProject> m_project;
  int m_project_modified_count{0};
};

//! Initial state of ProjectManager. Project created, and not-saved.
TEST_F(ProjectManagerFolderTests, InitialState)
{
  const ProjectManager manager(GetProject(ProjectType::kFolderBased));
  EXPECT_TRUE(manager.CurrentProjectPath().empty());
  EXPECT_FALSE(manager.IsModified());
}

// ----------------------------------------------------------------------------
// Untitled, empty project
// ----------------------------------------------------------------------------

//! Creating new project. Use untitled+empty project as a starting point. Should succeed, since old
//! empty project doesn't need to be saved.
TEST_F(ProjectManagerFolderTests, UntitledEmptyNew)
{
  ProjectManager manager(GetProject(ProjectType::kFolderBased));

  const auto project_dir = CreateEmptyDir("Project_untitledEmptyNew");
  EXPECT_TRUE(manager.CreateNewProject(project_dir));

  EXPECT_EQ(manager.CurrentProjectPath(), project_dir);
  EXPECT_FALSE(manager.IsModified());

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

//! Saving of new project. Use untitled+empty project as a starting point. Should fail since project
//! directory is not defined.
TEST_F(ProjectManagerFolderTests, UntitledEmptySave)
{
  ProjectManager manager(GetProject(ProjectType::kFolderBased));
  EXPECT_FALSE(manager.SaveCurrentProject());
  EXPECT_FALSE(manager.IsModified());
}

//! Saving of new project. Use untitled+empty project as a starting point. Should be saved, file
//! sould appear on disk.
TEST_F(ProjectManagerFolderTests, UntitledEmptySaveAs)
{
  ProjectManager manager(GetProject(ProjectType::kFolderBased));

  const auto project_dir = CreateEmptyDir("Project_untitledEmptySaveAs");
  EXPECT_TRUE(manager.SaveProjectAs(project_dir));
  EXPECT_FALSE(manager.IsModified());

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

// ----------------------------------------------------------------------------
// Untitled, modified
// ----------------------------------------------------------------------------

//! Creating new project. Use untitled+modified project as a starting point. Should fail, since
//! modified old project will prevent creation of the new one.
TEST_F(ProjectManagerFolderTests, UntitledModifiedNew)
{
  ProjectManager manager(GetProject(ProjectType::kFolderBased));

  // modifying the model
  sample_model->InsertItem<PropertyItem>();

  EXPECT_TRUE(manager.IsModified());

  const auto project_dir = CreateEmptyDir("Project_untitledModifiedNew");
  EXPECT_FALSE(manager.CreateNewProject(project_dir));

  EXPECT_TRUE(manager.CurrentProjectPath().empty());
  EXPECT_TRUE(manager.IsModified());

  // project directory should be empty
  auto model_filename = utils::Join(project_dir, kSampleModelName + ".xml");
  EXPECT_FALSE(utils::IsExists(model_filename));
}

//! Saving of new project. Use untitled+modified project as a starting point. Should fail since
//! project directory is not defined.
TEST_F(ProjectManagerFolderTests, UntitledModifiedSave)
{
  ProjectManager manager(GetProject(ProjectType::kFolderBased));
  // modifying the model
  sample_model->InsertItem<PropertyItem>();

  EXPECT_FALSE(manager.SaveCurrentProject());
  EXPECT_TRUE(manager.IsModified());
}

//! Saving of new project. Use untitled+empty project as a starting point. Should be saved, file
//! sould appear on disk.
TEST_F(ProjectManagerFolderTests, UntitledModifiedSaveAs)
{
  ProjectManager manager(GetProject(ProjectType::kFolderBased));
  sample_model->InsertItem<PropertyItem>();  // modifying the model

  const auto project_dir = CreateEmptyDir("Project_untitledModifiedSaveAs");
  EXPECT_TRUE(manager.SaveProjectAs(project_dir));
  EXPECT_FALSE(manager.IsModified());

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

//! File based version.
TEST_F(ProjectManagerFolderTests, FileBasedUntitledModifiedSaveAs)
{
  ProjectManager manager(GetProject(ProjectType::kFileBased));
  sample_model->InsertItem<PropertyItem>();  // modifying the model

  auto path = GetFilePath("FileBasedUntitledModifiedSaveAs.xml");

  EXPECT_TRUE(manager.SaveProjectAs(path));
  EXPECT_FALSE(manager.IsModified());

  // project directory should contain a file with the model
  EXPECT_TRUE(utils::IsExists(path));
}

// ----------------------------------------------------------------------------
// Titled, unmodified
// ----------------------------------------------------------------------------

//! Creating new project. Use titled+unmodified project as a starting point. Should succeed, since
//! old empty project doesn't need to be saved.
TEST_F(ProjectManagerFolderTests, TitledUnmodifiedNew)
{
  ProjectManager manager(GetProject(ProjectType::kFolderBased));

  const auto project_dir = CreateEmptyDir("Project_titledUnmodifiedNew");
  EXPECT_TRUE(manager.SaveProjectAs(project_dir));
  EXPECT_EQ(manager.CurrentProjectPath(), project_dir);

  const auto project_dir2 = CreateEmptyDir("Project_titledUnmodifiedNew2");
  EXPECT_TRUE(manager.CreateNewProject(project_dir2));

  EXPECT_EQ(manager.CurrentProjectPath(), project_dir2);
  EXPECT_FALSE(manager.IsModified());

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir2, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

// ----------------------------------------------------------------------------
// Titled, modified
// ----------------------------------------------------------------------------

//! Saving of new project. Use titled+modified project as a starting point. Should succeed.
TEST_F(ProjectManagerFolderTests, TitledModifiedSave)
{
  ProjectManager manager(GetProject(ProjectType::kFolderBased));

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

TEST_F(ProjectManagerFolderTests, Callback)
{
  auto context = GetProject(ProjectType::kFolderBased);

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
