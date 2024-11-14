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

#include "mvvm/project/file_based_project.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/project/project_context.h>
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

using namespace mvvm;

namespace
{
const std::string kSampleModelName = "SampleModel";
const std::string kMaterialModelName = "MaterialModel";
}  // namespace

//! Tests for FileBasedProject class.

class FileBasedProjectTest : public mvvm::test::FolderTest
{
public:
  FileBasedProjectTest()
      : FolderTest("FileBasedProjectTest")
      , m_sample_model(std::make_unique<ApplicationModel>(kSampleModelName))
      , m_material_model(std::make_unique<ApplicationModel>(kMaterialModelName))
  {
  }

  std::vector<ISessionModel*> GetModels() const
  {
    return {m_sample_model.get(), m_material_model.get()};
  };

  ProjectContext CreateContext() { return {}; }

  std::unique_ptr<ApplicationModel> m_sample_model;
  std::unique_ptr<ApplicationModel> m_material_model;
};

TEST_F(FileBasedProjectTest, InitialState)
{
  const FileBasedProject project(GetModels(), CreateContext());
  EXPECT_TRUE(project.GetProjectPath().empty());
  EXPECT_FALSE(project.IsModified());
  EXPECT_EQ(project.GetProjectType(), ProjectType::kFileBased);
}

//! Testing model saving. It should be a single file for all models.
TEST_F(FileBasedProjectTest, SaveModel)
{
  FileBasedProject project(GetModels(), CreateContext());

  const std::string file_name("untitled.xml");
  const std::string expected_path = mvvm::utils::Join(GetTestHomeDir(), file_name);

  project.Save(expected_path);

  EXPECT_EQ(project.GetProjectPath(), expected_path);
  EXPECT_FALSE(project.IsModified());

  EXPECT_TRUE(utils::IsExists(expected_path));

  auto files_in_dir = utils::FindFiles(GetTestHomeDir(), ".xml");
  ASSERT_EQ(files_in_dir.size(), 1);
  EXPECT_EQ(expected_path, files_in_dir.at(0));
}

//! Testing model loading.
TEST_F(FileBasedProjectTest, LoadModel)
{
  FileBasedProject project(GetModels(), CreateContext());

  auto item0 = m_sample_model->InsertItem<PropertyItem>();
  item0->SetData(std::string("sample_model_item"));
  auto item0_identifier = item0->GetIdentifier();

  auto item1 = m_material_model->InsertItem<PropertyItem>();
  item1->SetData(std::string("material_model_item"));
  auto item1_identifier = item1->GetIdentifier();

  // create project directory and save file
  auto subfolder = CreateEmptyDir("SubFolder");
  const std::string file_name("untitled2.oact");
  const std::string expected_path = mvvm::utils::Join(subfolder, file_name);

  EXPECT_TRUE(project.IsModified());
  project.Save(expected_path);
  EXPECT_FALSE(project.IsModified());

  EXPECT_EQ(project.GetProjectPath(), expected_path);

  // cleaning models
  m_sample_model->Clear();
  m_material_model->Clear();
  EXPECT_EQ(m_sample_model->GetRootItem()->GetTotalItemCount(), 0);
  EXPECT_EQ(m_material_model->GetRootItem()->GetTotalItemCount(), 0);
  EXPECT_TRUE(project.IsModified());

  // loading
  project.Load(expected_path);
  EXPECT_EQ(m_sample_model->GetRootItem()->GetTotalItemCount(), 1);
  EXPECT_EQ(m_material_model->GetRootItem()->GetTotalItemCount(), 1);

  // checking identifiers
  EXPECT_EQ(m_sample_model->GetRootItem()->GetAllItems()[0]->GetIdentifier(), item0_identifier);
  EXPECT_EQ(m_material_model->GetRootItem()->GetAllItems()[0]->GetIdentifier(), item1_identifier);

  EXPECT_EQ(project.GetProjectPath(), expected_path);
  EXPECT_FALSE(project.IsModified());
}
