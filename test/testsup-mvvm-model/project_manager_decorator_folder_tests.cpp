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
#include <mvvm/test/folder_based_test.h>
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>

using namespace mvvm;

namespace
{
const std::string kSampleModelName = "samplemodel";

}  // namespace

//! Tests for ProjectManagerDecorator class for folder-based documents.

class ProjectManagerDecoratorFolderTest : public mvvm::test::FolderBasedTest
{
public:
  ProjectManagerDecoratorFolderTest()
      : FolderBasedTest("test_ProjectManagerDecorator")
      , sample_model(std::make_unique<ApplicationModel>(kSampleModelName))
  {
  }

  std::vector<SessionModelInterface*> GetModels() const { return {sample_model.get()}; };

  std::function<std::unique_ptr<IProject>()> CreateContext()
  {
    auto result = [this]() -> std::unique_ptr<IProject>
    {
      ProjectContext context;
      context.m_models_callback = [this]() { return GetModels(); };
      return mvvm::utils::CreateUntitledFolderBasedProject(context);
    };

    return result;
  }

  static UserInteractionContext CreateUserContext(const std::string& create_dir = {},
                                                  const std::string& select_dir = {})
  {
    UserInteractionContext result;
    result.m_create_dir_callback = [create_dir]() -> std::string { return create_dir; };
    result.m_select_dir_callback = [select_dir]() -> std::string { return select_dir; };
    return result;
  }

  std::unique_ptr<IProjectManager> CreateProjectManager(const std::string& create_dir = {},
                                                        const std::string& select_dir = {})
  {
    auto project_manager = std::make_unique<ProjectManager>(CreateContext());
    return std::make_unique<ProjectManagerDecorator>(std::move(project_manager),
                                                     CreateUserContext(create_dir, select_dir));
  }

  std::unique_ptr<ApplicationModel> sample_model;
};

//! Initial state of ProjectManager. Project created, and not-saved.

TEST_F(ProjectManagerDecoratorFolderTest, InitialState)
{
  auto manager = CreateProjectManager();
  EXPECT_TRUE(manager->CurrentProjectPath().empty());
}

//! Starting from new document (without project dir defined).
//! Create new project in given directory.

TEST_F(ProjectManagerDecoratorFolderTest, UntitledEmptyCreateNew)
{
  const auto project_dir = CreateEmptyDir("Project_untitledEmptyCreateNew");

  auto manager = CreateProjectManager(project_dir);
  EXPECT_TRUE(manager->CurrentProjectPath().empty());

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

  auto manager = CreateProjectManager(project_dir);
  EXPECT_TRUE(manager->CurrentProjectPath().empty());

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

  auto manager = CreateProjectManager(project_dir);
  EXPECT_TRUE(manager->CurrentProjectPath().empty());

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

  // saving new project to "project_dir" directory.
  EXPECT_FALSE(manager->SaveProjectAs({}));
  EXPECT_TRUE(manager->CurrentProjectPath().empty());
}

//! Starting from new document (without project dir defined).
//! Attempt to save in the non-existing directory.

TEST_F(ProjectManagerDecoratorFolderTest, UntitledEmptySaveAsWrongDir)
{
  auto manager = CreateProjectManager("non-existing", {});

  // saving new project to "project_dir" directory.
  EXPECT_FALSE(manager->SaveProjectAs({}));
  EXPECT_TRUE(manager->CurrentProjectPath().empty());
}

//! Untitled, modified document. Attempt to open existing project will lead to
//! the dialog save/discard/cancel. As a result of whole exersize, existing project
//! should be opened, previous project saved.

TEST_F(ProjectManagerDecoratorFolderTest, UntitledModifiedOpenExisting)
{
  const auto existing_project_dir = CreateEmptyDir("Project_untitledModifiedOpenExisting1");
  const auto unsaved_project_dir = CreateEmptyDir("Project_untitledModifiedOpenExisting2");

  // create "existing project"
  {
    auto manager = CreateProjectManager(existing_project_dir, {});
    manager->SaveProjectAs({});
  }

  // preparing manager with untitled, unmodified project
  auto open_dir = [existing_project_dir]() -> std::string { return existing_project_dir; };
  auto create_dir = [unsaved_project_dir]() -> std::string { return unsaved_project_dir; };
  auto result = SaveChangesAnswer::kDiscard;
  auto ask_create = [&result]()
  {
    result = SaveChangesAnswer::kSave;
    return SaveChangesAnswer::kSave;
  };
  auto user_context = CreateUserContext({}, {});
  user_context.m_create_dir_callback = create_dir;
  user_context.m_select_dir_callback = open_dir;
  user_context.m_answer_callback = ask_create;

  auto project_manager = std::make_unique<ProjectManager>(CreateContext());
  ProjectManagerDecorator manager(std::move(project_manager), user_context);

  // modifying untitled project
  sample_model->InsertItem<PropertyItem>();
  EXPECT_TRUE(manager.IsModified());
  EXPECT_TRUE(manager.CurrentProjectPath().empty());

  // attempt to open existing project
  manager.OpenExistingProject({});

  // check if user was asked and his answer coincide with expectation
  EXPECT_EQ(result, SaveChangesAnswer::kSave);

  // check that previous project was saved
  auto model_filename = utils::Join(unsaved_project_dir, kSampleModelName + ".xml");
  EXPECT_TRUE(utils::IsExists(model_filename));

  // currently manager is pointing to existing project
  EXPECT_FALSE(manager.IsModified());
  EXPECT_EQ(manager.CurrentProjectPath(), existing_project_dir);
}
