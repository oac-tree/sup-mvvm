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

#include "mvvm/serialization/xml_document_helper.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>

#include <gtest/gtest.h>
#include <testutils/folder_test.h>

namespace mvvm
{

namespace
{
const std::string ModelTypeA = "ModelTypeA";
const std::string ModelTypeB = "ModelTypeB";
}  // namespace

/**
 * @brief Tests for methods in xml_document_helper.h
 */
class XmlDocumentHelperTest : public mvvm::test::FolderTest
{
public:
  XmlDocumentHelperTest() : FolderTest("XmlDocumentHelperTest") {}

  class TestModelA : public ApplicationModel
  {
  public:
    TestModelA() : ApplicationModel(ModelTypeA) {}
  };

  class TestModelB : public ApplicationModel
  {
  public:
    TestModelB() : ApplicationModel(ModelTypeB) {}
  };

  /**
   * @brief Writes given models to a file.
   */
  static void WriteModels(const std::vector<ISessionModel*>& models, const std::string& path,
                          const std::string& application_type = {})
  {
    const XmlDocument document(models, application_type);
    document.Save(path);
  }
};

TEST_F(XmlDocumentHelperTest, OneModel)
{
  const auto file_path = GetFilePath("OneModel.xml");

  {
    auto model = std::make_unique<TestModelA>();
    model->InsertItem<SessionItem>()->SetData(42);
    WriteModels({model.get()}, file_path);
  }

  auto result = LoadModels<TestModelA>(file_path);

  auto& loaded_model = std::get<0>(result);
  EXPECT_NE(dynamic_cast<TestModelA*>(loaded_model.get()), nullptr);

  ASSERT_NE(loaded_model->GetRootItem()->GetItem(TagIndex::First()), nullptr);
  EXPECT_EQ(loaded_model->GetRootItem()->GetItem(TagIndex::First())->Data<int>(), 42);
}

TEST_F(XmlDocumentHelperTest, TwoModels)
{
  const std::string application_type = {"TestApplication"};
  const auto file_path = GetFilePath("TwoModels.xml");

  {
    auto model0 = std::make_unique<TestModelA>();
    model0->InsertItem<SessionItem>()->SetData(42);

    auto model1 = std::make_unique<TestModelB>();
    model1->InsertItem<SessionItem>()->SetData(43);

    WriteModels({model0.get(), model1.get()}, file_path, application_type);
  }

  auto result = LoadModels<TestModelA, TestModelB>(file_path, application_type);

  auto loaded_model0 = std::get<0>(result).get();
  auto loaded_model1 = std::get<1>(result).get();
  EXPECT_NE(dynamic_cast<TestModelA*>(loaded_model0), nullptr);
  EXPECT_NE(dynamic_cast<TestModelB*>(loaded_model1), nullptr);

  EXPECT_EQ(loaded_model0->GetType(), ModelTypeA);
  EXPECT_EQ(loaded_model1->GetType(), ModelTypeB);

  ASSERT_NE(loaded_model0->GetRootItem()->GetItem(TagIndex::First()), nullptr);
  EXPECT_EQ(loaded_model0->GetRootItem()->GetItem(TagIndex::First())->Data<int>(), 42);

  ASSERT_NE(loaded_model1->GetRootItem()->GetItem(TagIndex::First()), nullptr);
  EXPECT_EQ(loaded_model1->GetRootItem()->GetItem(TagIndex::First())->Data<int>(), 43);
}

}  // namespace mvvm
