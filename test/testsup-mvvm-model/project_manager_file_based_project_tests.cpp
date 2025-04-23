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

#include "mvvm/project/project_manager.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/project/i_project.h>
#include <mvvm/project/project_context.h>
#include <mvvm/project/project_utils.h>
#include <mvvm/test/mock_project_context.h>
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>
#include <testutils/mock_user_interactor.h>

namespace mvvm
{

namespace
{
const std::string kSampleModelName = "samplemodel";
}  // namespace

/**
 * @brief Tests for ProjectManager class for file-based projects.
 */
class ProjectManagerFileBasedProjectTest : public mvvm::test::FolderTest
{
public:
  ProjectManagerFileBasedProjectTest()
      : FolderTest("ProjectManagerFileBasedTest")
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
   * @brief Creates project context with callbacks to preort project staus change.
   */
  ProjectContext CreateProjectContext() { return m_mock_project_context.CreateContext(); }

  /**
   * @brief Creates project manager.
   */
  std::unique_ptr<IProjectManager> CreateProjectManager(const std::string& new_path = {},
                                                        const std::string& existing_path = {})
  {
    m_project = mvvm::utils::CreateUntitledProject(ProjectType::kFileBased, GetModels(),
                                                   CreateProjectContext());

    return std::make_unique<ProjectManager>(m_project.get(),
                                            CreateUserContext(new_path, existing_path));
  }

  std::unique_ptr<ApplicationModel> m_sample_model;
  test::MockUserInteractor m_mock_interactor;
  test::MockProjectContext m_mock_project_context;
  std::unique_ptr<IProject> m_project;
};

//! Saving untitled modified project under the given name.
TEST_F(ProjectManagerFileBasedProjectTest, UntitledModifiedSaveAs)
{
  auto manager = CreateProjectManager({}, {});

  EXPECT_CALL(m_mock_project_context, OnModified()).Times(1);
  EXPECT_CALL(m_mock_project_context, OnLoaded()).Times(0);
  EXPECT_CALL(m_mock_project_context, OnSaved()).Times(1);

  m_sample_model->InsertItem<PropertyItem>();  // modifying the model

  auto path = GetFilePath("FileBasedUntitledModifiedSaveAs.xml");

  EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnMessage(testing::_)).Times(0);

  EXPECT_TRUE(manager->SaveProjectAs(path));
  EXPECT_FALSE(manager->GetProject()->IsModified());

  // project directory should contain a file with the model
  EXPECT_TRUE(utils::IsExists(path));
}

//! Opening non-existing file.
TEST_F(ProjectManagerFileBasedProjectTest, AttemptToOpenNonExistingFile)
{
  auto manager = CreateProjectManager({}, {});

  auto path = GetFilePath("no-such-file.xml");

  EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(0);

  // expection will be thrown, caught, and turned into the message
  EXPECT_CALL(m_mock_interactor, OnMessage(testing::_)).Times(1);

  EXPECT_FALSE(manager->OpenExistingProject(path));
  EXPECT_FALSE(manager->GetProject()->IsModified());
}

//! Starting from new document (without project dir defined). Attempt to save under empty name,
//! imitating the user canceled directory selection dialog.
TEST_F(ProjectManagerFileBasedProjectTest, UntitledEmptySaveAsCancel)
{
  auto manager = CreateProjectManager({}, {});  // imitates dialog canceling
  EXPECT_FALSE(manager->GetProject()->HasPath());

  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);

  // saving new project to "project_dir" directory.
  EXPECT_FALSE(manager->SaveProjectAs({}));
  EXPECT_FALSE(manager->GetProject()->HasPath());
}

//! Untitled modified project. User decides to create new project, and discards all previous
//! changes. As a result, new XML file should appear on disk, the model should be cleared.
TEST_F(ProjectManagerFileBasedProjectTest, UntitledModifiedDiscardAndCreateNew)
{
  auto new_path = GetFilePath("UntitledModifiedDiscardAndCreateNew.xml");

  // instructing mock interactor to return necessary values
  ON_CALL(m_mock_interactor, OnSaveChangesRequest())
      .WillByDefault(::testing::Return(SaveChangesAnswer::kDiscard));

  auto manager = CreateProjectManager(new_path, {});

  EXPECT_CALL(m_mock_project_context, OnModified()).Times(1);
  m_sample_model->InsertItem<PropertyItem>();  // modifying the model

  // setting up expectations: attempt to open existing project will lead to the following chain
  // 1) question whether to save currently modified project
  // 2) request to open new project
  EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(1);
  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);
  EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(0);

  EXPECT_CALL(m_mock_project_context, OnLoaded()).Times(1);
  EXPECT_CALL(m_mock_project_context, OnSaved()).Times(1);
  EXPECT_TRUE(manager->CreateNewProject({}));

  // new project file should be there
  EXPECT_TRUE(utils::IsExists(new_path));

  // model should be empty
  EXPECT_EQ(m_sample_model->GetRootItem()->GetTotalItemCount(), 0);
}

//! Untitled modified project. User decides to create new project, and discards all previous
//! changes. While selecting the name for the new file he suddenly push "Cancel". As a result old
//! content should remain, the project should be in modified state.
TEST_F(ProjectManagerFileBasedProjectTest, UntitledModifiedDiscardAndCreateNewButThenCancel)
{
  // instructing mock interactor to return necessary values
  ON_CALL(m_mock_interactor, OnSaveChangesRequest())
      .WillByDefault(::testing::Return(SaveChangesAnswer::kDiscard));

  // empty path denotes "Cancel" button during file name selection
  auto manager = CreateProjectManager("", {});

  EXPECT_CALL(m_mock_project_context, OnModified()).Times(1);
  m_sample_model->InsertItem<PropertyItem>();  // modifying the model

  // setting up expectations: attempt to open existing project will lead to the following chain
  // 1) question whether to save currently modified project
  // 2) request to open new project
  EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(1);
  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);
  EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(0);

  EXPECT_FALSE(manager->CreateNewProject({}));

  // model should be still full
  EXPECT_EQ(m_sample_model->GetRootItem()->GetTotalItemCount(), 1);
}

//! Untitled modified project. User decides to load existing project, and discards all previous
//! changes. While selecting the name for existing project user suddenly pushes "Cancel". As a
//! result old content should remain, the project should be in modified state.
//! Real-life bug.
TEST_F(ProjectManagerFileBasedProjectTest,
       UntitledModifiedDiscardAndLoadExistingProjectButThenCancel)
{
  // instructing mock interactor to return necessary values
  ON_CALL(m_mock_interactor, OnSaveChangesRequest())
      .WillByDefault(::testing::Return(SaveChangesAnswer::kDiscard));

  // empty path denotes "Cancel" button during file name selection
  auto manager = CreateProjectManager(/*new file name*/ {},
                                      /*existing file name*/ {});

  EXPECT_CALL(m_mock_project_context, OnModified()).Times(1);
  m_sample_model->InsertItem<PropertyItem>();  // modifying the model

  // setting up expectations: attempt to open existing project will lead to the following chain
  // 1) question whether to save currently modified project
  // 2) request to open new project
  EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(1);
  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(0);
  EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(1);

  // Pretending we want to open existing project. By providing empty name we trigger file selection
  // dialog.
  EXPECT_FALSE(manager->OpenExistingProject(""));

  // model should be still full
  EXPECT_EQ(m_sample_model->GetRootItem()->GetTotalItemCount(), 1);
}

}  // namespace mvvm
