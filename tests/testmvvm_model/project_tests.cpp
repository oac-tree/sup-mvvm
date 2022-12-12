/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/project/project.h"

#include <gtest/gtest.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>
#include <mvvm/project/project_types.h>
#include <mvvm/utils/file_utils.h>
#include <testutils/folder_based_test.h>

#include <cctype>

using namespace mvvm;

namespace
{
const std::string samplemodel_name = "SampleModel";
const std::string materialmodel_name = "MaterialModel";

//! Constructs XML file name from SessionModel typeName (as it is done internaly by Project).
std::string GetXmlFilename(const std::string& model_name)
{
  std::string result(model_name);
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result + ".xml";
}

}  // namespace

//! Tests for Project class.

class ProjectTests : public testutils::FolderBasedTest
{
public:
  ProjectTests()
      : FolderBasedTest("test_ProjectTests")
      , sample_model(std::make_unique<ApplicationModel>(samplemodel_name))
      , material_model(std::make_unique<ApplicationModel>(materialmodel_name))
  {
  }

  std::vector<SessionModelInterface*> models() const
  {
    return {sample_model.get(), material_model.get()};
  };

  ProjectContext CreateContext()
  {
    ProjectContext result;
    result.m_models_callback = [this]() { return models(); };
    return result;
  }

  std::unique_ptr<ApplicationModel> sample_model;
  std::unique_ptr<ApplicationModel> material_model;
};

TEST_F(ProjectTests, InitialState)
{
  Project project(CreateContext());
  EXPECT_TRUE(project.GetProjectDir().empty());
  EXPECT_FALSE(project.IsModified());
}

//! Testing saveModel.

TEST_F(ProjectTests, SaveModel)
{
  Project project(CreateContext());

  // create project directory and save file
  auto project_dir = CreateEmptyDir("Untitled1");
  project.Save(project_dir);

  EXPECT_EQ(project.GetProjectDir(), project_dir);
  EXPECT_FALSE(project.IsModified());

  auto sample_xml = utils::Join(project_dir, GetXmlFilename(samplemodel_name));
  EXPECT_TRUE(utils::IsExists(sample_xml));

  auto material_xml = utils::Join(project_dir, GetXmlFilename(materialmodel_name));
  EXPECT_TRUE(utils::IsExists(material_xml));
}

//! Testing loadModel.

TEST_F(ProjectTests, LoadModel)
{
  Project project(CreateContext());

  auto item0 = sample_model->InsertItem<PropertyItem>();
  item0->SetData(std::string("sample_model_item"));
  auto item0_identifier = item0->GetIdentifier();

  auto item1 = material_model->InsertItem<PropertyItem>();
  item1->SetData(std::string("material_model_item"));
  auto item1_identifier = item1->GetIdentifier();

  // create project directory and save file
  auto project_dir = CreateEmptyDir("Untitled2");

  EXPECT_TRUE(project.IsModified());
  project.Save(project_dir);
  EXPECT_FALSE(project.IsModified());

  EXPECT_EQ(project.GetProjectDir(), project_dir);

  // cleaning models
  sample_model->Clear({});
  material_model->Clear({});
  EXPECT_EQ(sample_model->GetRootItem()->GetTotalItemCount(), 0);
  EXPECT_EQ(material_model->GetRootItem()->GetTotalItemCount(), 0);
  EXPECT_TRUE(project.IsModified());

  // loading
  project.Load(project_dir);
  EXPECT_EQ(sample_model->GetRootItem()->GetTotalItemCount(), 1);
  EXPECT_EQ(material_model->GetRootItem()->GetTotalItemCount(), 1);

  // checking identifiers
  EXPECT_EQ(sample_model->GetRootItem()->GetAllItems()[0]->GetIdentifier(), item0_identifier);
  EXPECT_EQ(material_model->GetRootItem()->GetAllItems()[0]->GetIdentifier(), item1_identifier);

  EXPECT_EQ(project.GetProjectDir(), project_dir);
  EXPECT_FALSE(project.IsModified());
}
