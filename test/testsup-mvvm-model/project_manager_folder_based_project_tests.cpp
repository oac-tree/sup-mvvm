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
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>
#include <testutils/mock_user_interactor.h>

using namespace mvvm;

namespace
{
const std::string kSampleModelName = "samplemodel";

}  // namespace

//! Tests for ProjectManager class for folder-based documents.

/**
 * @brief Tests for ProjectManager class for.
 */
class ProjectManagerFolderTest : public mvvm::test::FolderTest
{
public:
  ProjectManagerFolderTest()
      : FolderTest("ProjectManagerFolderTest")
      , m_sample_model(std::make_unique<ApplicationModel>(kSampleModelName))
  {
  }

  std::vector<ISessionModel*> GetModels() const { return {m_sample_model.get()}; };

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

  /**
   * @brief Creates project manager decorator.
   */
  std::unique_ptr<IProjectManager> CreateProjectManager(const std::string& new_path = {},
                                                        const std::string& existing_path = {})
  {
    ProjectContext context;
    context.modified_callback = m_modified_callback.AsStdFunction();
    context.loaded_callback = m_loaded_callback.AsStdFunction();
    m_project = mvvm::utils::CreateUntitledProject(ProjectType::kFolderBased, GetModels(), context);

    return std::make_unique<ProjectManager>(m_project.get(),
                                            CreateUserContext(new_path, existing_path));
  }

  std::unique_ptr<ApplicationModel> m_sample_model;
  test::MockUserInteractor m_mock_interactor;
  std::unique_ptr<IProject> m_project;
  ::testing::MockFunction<void()> m_modified_callback;
  ::testing::MockFunction<void()> m_loaded_callback;
};

//! Initial state of ProjectManager. Project created, and not-saved.
TEST_F(ProjectManagerFolderTest, InitialState)
{
  auto manager = CreateProjectManager();
  EXPECT_FALSE(manager->GetProject()->HasPath());
  EXPECT_FALSE(manager->IsModified());
}

//! Creating new project. Use untitled+empty project as a starting point. Should succeed, since old
//! empty project doesn't need to be saved.
TEST_F(ProjectManagerFolderTest, CreateNewStartingFromUntitledEmptyProject)
{
  auto manager = CreateProjectManager({}, {});

  const auto project_dir = CreateEmptyDir("Project_untitledEmptyNew");

  // because File/Folder based projects make clear() on project close
  EXPECT_CALL(m_modified_callback, Call()).Times(1);  // on project close
  EXPECT_CALL(m_loaded_callback, Call()).Times(1);    // on project creation

  EXPECT_TRUE(manager->CreateNewProject(project_dir));

  EXPECT_EQ(manager->GetProject()->GetPath(), project_dir);
  EXPECT_FALSE(manager->IsModified());

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

//! Starting from new document (without project dir defined). Create new project in given directory.
TEST_F(ProjectManagerFolderTest, UntitledEmptyCreateNew)
{
  const auto project_dir = CreateEmptyDir("Project_untitledEmptyCreateNew");

  auto manager = CreateProjectManager(project_dir);
  EXPECT_FALSE(manager->GetProject()->HasPath());

  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);
  EXPECT_CALL(m_modified_callback, Call()).Times(1);
  EXPECT_CALL(m_loaded_callback, Call()).Times(1);

  // saving new project to 'project_dir' directory.
  EXPECT_TRUE(manager->CreateNewProject({}));

  // checking that current projectDir has pointing to the right place
  EXPECT_EQ(manager->GetProject()->GetPath(), project_dir);

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

//! Starting from new document (without project dir defined). Saving project. Same behavior as
//! SaveAs.
TEST_F(ProjectManagerFolderTest, UntitledEmptySaveCurrentProject)
{
  const auto project_dir = CreateEmptyDir("Project_untitledEmptySaveCurrentProject");

  auto manager = CreateProjectManager(project_dir);
  EXPECT_FALSE(manager->GetProject()->HasPath());

  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);

  // saving new project to 'project_dir' directory.
  EXPECT_TRUE(manager->SaveCurrentProject());

  // checking thaxt current projectDir has pointing to the right place
  EXPECT_EQ(manager->GetProject()->GetPath(), project_dir);

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

//! Starting from new document (without project dir defined). Save under given name.
TEST_F(ProjectManagerFolderTest, UntitledEmptySaveAs)
{
  const auto project_dir = CreateEmptyDir("Project_untitledEmptySaveAs");

  auto manager = CreateProjectManager(project_dir);
  EXPECT_FALSE(manager->GetProject()->HasPath());

  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);

  // saving new project to "project_dir" directory.
  EXPECT_TRUE(manager->SaveProjectAs({}));

  // checking that current projectDir has pointing to the right place
  EXPECT_EQ(manager->GetProject()->GetPath(), project_dir);

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

//! Starting from new document (without project dir defined). Attempt to save under empty name,
//! imitating the user canceled directory selection dialog.
TEST_F(ProjectManagerFolderTest, UntitledEmptySaveAsCancel)
{
  auto manager = CreateProjectManager({}, {});  // imitates dialog canceling
  EXPECT_FALSE(manager->GetProject()->HasPath());

  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);

  // saving new project to "project_dir" directory.
  EXPECT_FALSE(manager->SaveProjectAs({}));
  EXPECT_FALSE(manager->GetProject()->HasPath());
}

//! Starting from new document (without project dir defined).
//! Attempt to save in the non-existing directory.
TEST_F(ProjectManagerFolderTest, UntitledEmptySaveAsWrongDir)
{
  auto manager = CreateProjectManager("non-existing", {});

  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);

  // saving new project to "project_dir" directory.
  EXPECT_FALSE(manager->SaveProjectAs({}));
  EXPECT_FALSE(manager->GetProject()->HasPath());
}

//! Untitled, modified document. Attempt to open an existing project will lead to
//! the dialog save/discard/cancel. As a result of the whole exercise, the existing project
//! should be opened, and the previous project saved.
TEST_F(ProjectManagerFolderTest, UntitledModifiedOpenExisting)
{
  const auto existing_project_dir = CreateEmptyDir("Project_untitledModifiedOpenExisting1");
  const auto unsaved_project_dir = CreateEmptyDir("Project_untitledModifiedOpenExisting2");

  // create "existing project"
  {
    EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);

    auto manager = CreateProjectManager(existing_project_dir, {});
    manager->SaveProjectAs({});
  }

  // instructing mock interactor to return necessary values
  ON_CALL(m_mock_interactor, OnSaveChangesRequest())
      .WillByDefault(::testing::Return(SaveChangesAnswer::kSave));

  EXPECT_CALL(m_loaded_callback, Call()).Times(1);

  auto manager = CreateProjectManager(unsaved_project_dir, existing_project_dir);

  // modifying untitled project
  EXPECT_CALL(m_modified_callback, Call()).Times(1);

  m_sample_model->InsertItem<PropertyItem>();
  EXPECT_TRUE(manager->IsModified());
  EXPECT_FALSE(manager->GetProject()->HasPath());

  // setting up expectations: attempt to open existing project will lead to the following chain
  // 1) question whether to save currently modified project
  // 2) request to select directory for changes
  // 3) request to open new project
  EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(1);
  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);
  EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(1);

  // attempt to open existing project
  EXPECT_CALL(m_modified_callback, Call()).Times(1);
  manager->OpenExistingProject({});

  // check that previous project was saved
  auto model_filename = utils::Join(unsaved_project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));

  // currently manager is pointing to existing project
  EXPECT_FALSE(manager->IsModified());
  EXPECT_EQ(manager->GetProject()->GetPath(), existing_project_dir);
}

//! Creating new project. Use titled+unmodified project as a starting point. Should succeed, since
//! old empty project doesn't need to be saved.
TEST_F(ProjectManagerFolderTest, TitledUnmodifiedNew)
{
  auto manager = CreateProjectManager({}, {});

  const auto project_dir = CreateEmptyDir("Project_titledUnmodifiedNew");
  EXPECT_TRUE(manager->SaveProjectAs(project_dir));
  EXPECT_EQ(manager->GetProject()->GetPath(), project_dir);

  EXPECT_CALL(m_modified_callback, Call()).Times(1);
  EXPECT_CALL(m_loaded_callback, Call()).Times(1);

  const auto project_dir2 = CreateEmptyDir("Project_titledUnmodifiedNew2");
  EXPECT_TRUE(manager->CreateNewProject(project_dir2));

  EXPECT_EQ(manager->GetProject()->GetPath(), project_dir2);
  EXPECT_FALSE(manager->IsModified());

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir2, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

//! Saving of new project. Use titled+modified project as a starting point. Should succeed.
TEST_F(ProjectManagerFolderTest, TitledModifiedSave)
{
  auto manager = CreateProjectManager({}, {});

  const auto project_dir = CreateEmptyDir("Project_titledModifiedSave");
  EXPECT_TRUE(manager->SaveProjectAs(project_dir));
  EXPECT_EQ(manager->GetProject()->GetPath(), project_dir);

  // modifying the model
  EXPECT_CALL(m_modified_callback, Call()).Times(1);
  m_sample_model->InsertItem<PropertyItem>();

  EXPECT_TRUE(manager->SaveCurrentProject());
  EXPECT_FALSE(manager->IsModified());
}
