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

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/project/i_project.h>
#include <mvvm/project/project_context.h>
#include <mvvm/project/project_manager.h>
#include <mvvm/project/project_utils.h>
#include <mvvm/test/folder_test.h>
#include <mvvm/test/mock_user_interactor.h>
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>

using namespace mvvm;

namespace
{
const std::string kSampleModelName = "samplemodel";

}  // namespace

//! Tests for ProjectManagerDecorator class for folder-based documents.

class ProjectManagerDecoratorFolderTest : public mvvm::test::FolderTest
{
public:
  ProjectManagerDecoratorFolderTest()
      : FolderTest(""), m_sample_model(std::make_unique<ApplicationModel>(kSampleModelName))
  {
  }

  std::vector<ISessionModel*> GetModels() const { return {m_sample_model.get()}; };

  /**
   * @brief Returns factory function to create projects.
   */
  std::function<std::unique_ptr<IProject>()> ProjectFactoryFunc(ProjectType project_type)
  {
    auto result = [this, project_type]() -> std::unique_ptr<IProject>
    {
      ProjectContext context;
      context.models_callback = [this]() { return GetModels(); };
      return mvvm::utils::CreateUntitledProject(project_type, context);
    };

    return result;
  }

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
  std::unique_ptr<IProjectManager> CreateProjectManager(ProjectType project_type,
                                                        const std::string& new_path = {},
                                                        const std::string& existing_path = {})
  {
    auto project_manager = std::make_unique<ProjectManager>(ProjectFactoryFunc(project_type));
    return std::make_unique<ProjectManagerDecorator>(std::move(project_manager),
                                                     CreateUserContext(new_path, existing_path));
  }

  std::unique_ptr<ApplicationModel> m_sample_model;
  test::MockUserInteractor m_mock_interactor;
};

//! Initial state of ProjectManager. Project created, and not-saved.

TEST_F(ProjectManagerDecoratorFolderTest, InitialState)
{
  auto manager = CreateProjectManager(ProjectType::kFolderBased);
  EXPECT_TRUE(manager->CurrentProjectPath().empty());
}

//! File based project. Saving untitled modified project under given name.

TEST_F(ProjectManagerDecoratorFolderTest, FileBasedUntitledModifiedSaveAs)
{
  auto manager = CreateProjectManager(ProjectType::kFileBased, {}, {});

  m_sample_model->InsertItem<PropertyItem>();  // modifying the model

  auto path = GetFilePath("FileBasedUntitledModifiedSaveAs.xml");

  EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnMessage(testing::_)).Times(0);

  EXPECT_TRUE(manager->SaveProjectAs(path));
  EXPECT_FALSE(manager->IsModified());

  // project directory should contain a file with the model
  EXPECT_TRUE(utils::IsExists(path));
}

//! File based project. Opening non-existing file.

TEST_F(ProjectManagerDecoratorFolderTest, AttemptToOpenNonExistingFile)
{
  auto manager = CreateProjectManager(ProjectType::kFileBased, {}, {});

  auto path = GetFilePath("no-such-file.xml");

  EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(0);
  EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(0);

  // expection will be thrown, caught, and turned into the message
  EXPECT_CALL(m_mock_interactor, OnMessage(testing::_)).Times(1);

  EXPECT_FALSE(manager->OpenExistingProject(path));
  EXPECT_FALSE(manager->IsModified());
}

//! Starting from new document (without project dir defined).
//! Create new project in given directory.

TEST_F(ProjectManagerDecoratorFolderTest, UntitledEmptyCreateNew)
{
  const auto project_dir = CreateEmptyDir("Project_untitledEmptyCreateNew");

  auto manager = CreateProjectManager(ProjectType::kFolderBased, project_dir);
  EXPECT_TRUE(manager->CurrentProjectPath().empty());

  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);

  // saving new project to 'project_dir' directory.
  EXPECT_TRUE(manager->CreateNewProject({}));

  // checking that current projectDir has pointing to the right place
  EXPECT_EQ(manager->CurrentProjectPath(), project_dir);

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

//! Starting from new document (without project dir defined).
//! Saving project. Same behavior as SaveAs.

TEST_F(ProjectManagerDecoratorFolderTest, UntitledEmptySaveCurrentProject)
{
  const auto project_dir = CreateEmptyDir("Project_untitledEmptySaveCurrentProject");

  auto manager = CreateProjectManager(ProjectType::kFolderBased, project_dir);
  EXPECT_TRUE(manager->CurrentProjectPath().empty());

  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);

  // saving new project to 'project_dir' directory.
  EXPECT_TRUE(manager->SaveCurrentProject());

  // checking thaxt current projectDir has pointing to the right place
  EXPECT_EQ(manager->CurrentProjectPath(), project_dir);

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

//! Starting from new document (without project dir defined).
//! Save under given name.

TEST_F(ProjectManagerDecoratorFolderTest, UntitledEmptySaveAs)
{
  const auto project_dir = CreateEmptyDir("Project_untitledEmptySaveAs");

  auto manager = CreateProjectManager(ProjectType::kFolderBased, project_dir);
  EXPECT_TRUE(manager->CurrentProjectPath().empty());

  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);

  // saving new project to "project_dir" directory.
  EXPECT_TRUE(manager->SaveProjectAs({}));

  // checking that current projectDir has pointing to the right place
  EXPECT_EQ(manager->CurrentProjectPath(), project_dir);

  // project directory should contain a file with the model
  auto model_filename = utils::Join(project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));
}

//! Starting from new document (without project dir defined).
//! Attempt to save under empty name, immitating the user canceled directory selection dialog.

TEST_F(ProjectManagerDecoratorFolderTest, UntitledEmptySaveAsCancel)
{
  auto manager = CreateProjectManager({}, {});  // imitates dialog canceling
  EXPECT_TRUE(manager->CurrentProjectPath().empty());

  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);

  // saving new project to "project_dir" directory.
  EXPECT_FALSE(manager->SaveProjectAs({}));
  EXPECT_TRUE(manager->CurrentProjectPath().empty());
}

//! Starting from new document (without project dir defined).
//! Attempt to save in the non-existing directory.

TEST_F(ProjectManagerDecoratorFolderTest, UntitledEmptySaveAsWrongDir)
{
  auto manager = CreateProjectManager(ProjectType::kFolderBased, "non-existing", {});

  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);

  // saving new project to "project_dir" directory.
  EXPECT_FALSE(manager->SaveProjectAs({}));
  EXPECT_TRUE(manager->CurrentProjectPath().empty());
}

//! Untitled, modified document. Attempt to open an existing project will lead to
//! the dialog save/discard/cancel. As a result of the whole exercise, the existing project
//! should be opened, and the previous project saved.
TEST_F(ProjectManagerDecoratorFolderTest, UntitledModifiedOpenExisting)
{
  const auto existing_project_dir = CreateEmptyDir("Project_untitledModifiedOpenExisting1");
  const auto unsaved_project_dir = CreateEmptyDir("Project_untitledModifiedOpenExisting2");

  // create "existing project"
  {
    EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);
    auto manager = CreateProjectManager(ProjectType::kFolderBased, existing_project_dir, {});
    manager->SaveProjectAs({});
  }

  // instructing mock interactor to return necessary values
  ON_CALL(m_mock_interactor, OnSaveChangesRequest())
      .WillByDefault(::testing::Return(SaveChangesAnswer::kSave));

  auto manager =
      CreateProjectManager(ProjectType::kFolderBased, unsaved_project_dir, existing_project_dir);

  // modifying untitled project
  m_sample_model->InsertItem<PropertyItem>();
  EXPECT_TRUE(manager->IsModified());
  EXPECT_TRUE(manager->CurrentProjectPath().empty());

  // setting up expectations: attempt to open existing project will lead to the following chain
  // 1) question whether to save currently modified project
  // 2) request to select directory for changes
  // 3) request to open new project
  EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(1);
  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);
  EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(1);

  // attempt to open existing project
  manager->OpenExistingProject({});

  // check that previous project was saved
  auto model_filename = utils::Join(unsaved_project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));

  // currently manager is pointing to existing project
  EXPECT_FALSE(manager->IsModified());
  EXPECT_EQ(manager->CurrentProjectPath(), existing_project_dir);
}

//! Untitled modified project. User decides to create new project, and discards all previous
//! changes. As a result, new XML file should appear on disk, the model should be cleared.

TEST_F(ProjectManagerDecoratorFolderTest, UntitledModifiedDiscardAndCreateNew)
{
  auto new_path = GetFilePath("UntitledModifiedDiscardAndCreateNew.xml");

  // instructing mock interactor to return necessary values
  ON_CALL(m_mock_interactor, OnSaveChangesRequest())
      .WillByDefault(::testing::Return(SaveChangesAnswer::kDiscard));

  auto manager = CreateProjectManager(ProjectType::kFileBased, new_path, {});

  m_sample_model->InsertItem<PropertyItem>();  // modifying the model

  // setting up expectations: attempt to open existing project will lead to the following chain
  // 1) question whether to save currently modified project
  // 2) request to open new project
  EXPECT_CALL(m_mock_interactor, OnSaveChangesRequest()).Times(1);
  EXPECT_CALL(m_mock_interactor, OnGetNewProjectPath()).Times(1);
  EXPECT_CALL(m_mock_interactor, GetExistingProjectPath()).Times(0);

  EXPECT_TRUE(manager->CreateNewProject({}));

  // new project file should be there
  EXPECT_TRUE(utils::IsExists(new_path));

  // model should be empty
  EXPECT_EQ(m_sample_model->GetRootItem()->GetTotalItemCount(), 0);
}
