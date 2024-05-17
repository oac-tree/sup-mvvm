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

#include "mvvm/project/folder_based_project.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/project/project_context.h>
#include <mvvm/test/folder_test.h>
#include <mvvm/utils/file_utils.h>

#include <gtest/gtest.h>

#include <cctype>

using namespace mvvm;

namespace
{
const std::string kSampleModelName = "SampleModel";
const std::string kMaterialModelName = "MaterialModel";

//! Constructs XML file name from SessionModel typeName (as it is done internaly by Project).
std::string GetXmlFilename(const std::string& model_name)
{
  std::string result(model_name);
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result + ".xml";
}

}  // namespace

//! Tests for FolderBasedProject class.

class FolderBasedProjectTest : public mvvm::test::FolderTest
{
public:
  FolderBasedProjectTest()
      : FolderTest("test_FolderBasedProject")
      , m_sample_model(std::make_unique<ApplicationModel>(kSampleModelName))
      , m_material_model(std::make_unique<ApplicationModel>(kMaterialModelName))
  {
  }

  std::vector<SessionModelInterface*> GetModels() const
  {
    return {m_sample_model.get(), m_material_model.get()};
  };

  ProjectContext CreateContext()
  {
    ProjectContext result;
    result.models_callback = [this]() { return GetModels(); };
    return result;
  }

  std::unique_ptr<ApplicationModel> m_sample_model;
  std::unique_ptr<ApplicationModel> m_material_model;
};

TEST_F(FolderBasedProjectTest, InitialState)
{
  FolderBasedProject project(CreateContext());
  EXPECT_TRUE(project.GetProjectPath().empty());
  EXPECT_FALSE(project.IsModified());
  EXPECT_EQ(project.GetProjectType(), ProjectType::kFolderBased);
}

//! Testing model saving.
TEST_F(FolderBasedProjectTest, SaveModel)
{
  FolderBasedProject project(CreateContext());

  // create project directory and save file
  auto project_dir = CreateEmptyDir("Untitled1");
  project.Save(project_dir);

  EXPECT_EQ(project.GetProjectPath(), project_dir);
  EXPECT_FALSE(project.IsModified());

  auto sample_xml = utils::Join(project_dir, GetXmlFilename(kSampleModelName));
  EXPECT_TRUE(utils::IsExists(sample_xml));

  auto material_xml = utils::Join(project_dir, GetXmlFilename(kMaterialModelName));
  EXPECT_TRUE(utils::IsExists(material_xml));
}

//! Testing model loading.
TEST_F(FolderBasedProjectTest, LoadModel)
{
  FolderBasedProject project(CreateContext());

  auto item0 = m_sample_model->InsertItem<PropertyItem>();
  item0->SetData(std::string("sample_model_item"));
  auto item0_identifier = item0->GetIdentifier();

  auto item1 = m_material_model->InsertItem<PropertyItem>();
  item1->SetData(std::string("material_model_item"));
  auto item1_identifier = item1->GetIdentifier();

  // create project directory and save file
  auto project_dir = CreateEmptyDir("Untitled2");

  EXPECT_TRUE(project.IsModified());
  project.Save(project_dir);
  EXPECT_FALSE(project.IsModified());

  EXPECT_EQ(project.GetProjectPath(), project_dir);

  // cleaning models
  m_sample_model->Clear({});
  m_material_model->Clear({});
  EXPECT_EQ(m_sample_model->GetRootItem()->GetTotalItemCount(), 0);
  EXPECT_EQ(m_material_model->GetRootItem()->GetTotalItemCount(), 0);
  EXPECT_TRUE(project.IsModified());

  // loading
  project.Load(project_dir);
  EXPECT_EQ(m_sample_model->GetRootItem()->GetTotalItemCount(), 1);
  EXPECT_EQ(m_material_model->GetRootItem()->GetTotalItemCount(), 1);

  // checking identifiers
  EXPECT_EQ(m_sample_model->GetRootItem()->GetAllItems()[0]->GetIdentifier(), item0_identifier);
  EXPECT_EQ(m_material_model->GetRootItem()->GetAllItems()[0]->GetIdentifier(), item1_identifier);

  EXPECT_EQ(project.GetProjectPath(), project_dir);
  EXPECT_FALSE(project.IsModified());
}
