/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/editors/variant_dependent_editor_factory.h"

#include <mvvm/editors/custom_editor_includes.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/session_item.h>
#include <mvvm/utils/i_limited_float.h>
#include <mvvm/utils/i_limited_integer.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>
#include <testutils/test_viewmodel_helper.h>


namespace mvvm::test
{

/**
 * @brief Tests for editor factories from custom_editor_factories header.
 */
class VariantDependentEditorFactoryTest : public ::testing::Test
{
public:
  VariantDependentEditorFactoryTest() : m_view_model(&m_model) {}

  /**
   * @brief Convenience function to add given data to the model as PropertyItem.
   */
  QModelIndex AddDataToModel(const variant_t& data, const std::string& editor_type = {})
  {
    return test::AddDataToModel(data, editor_type, m_view_model);
  }

  ApplicationModel m_model;
  AllItemsViewModel m_view_model;
};

TEST_F(VariantDependentEditorFactoryTest, VariantDependentEditorFactory)
{
  const VariantDependentEditorFactory factory;

  // editor for bool types
  auto index = AddDataToModel(variant_t(true));
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(factory.CreateEditor(index).get()));

  // ComboProperty
  index = AddDataToModel(variant_t(ComboProperty()));
  EXPECT_TRUE(dynamic_cast<ComboPropertyEditor*>(factory.CreateEditor(index).get()));

  // `string` doesn't have custom editor for the moment (handled by default delegate)
  index = AddDataToModel(std::string("abc"));
  EXPECT_FALSE(factory.CreateEditor(index));

  index = AddDataToModel(variant_t(42));
  EXPECT_TRUE(dynamic_cast<AllIntSpinBox*>(factory.CreateEditor(index).get()));

  {  // uint32
    const uint32 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBox*>(factory.CreateEditor(index).get()));
  }

  {  // int64
    const int64 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBox*>(factory.CreateEditor(index).get()));
  }

  {  // uint64
    const uint64 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBox*>(factory.CreateEditor(index).get()));
  }

  {  // float32
    const float32 num(42.1);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<ScientificSpinBox*>(factory.CreateEditor(index).get()));
  }

  {  // float64
    const float64 num(42.1);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<ScientificSpinBox*>(factory.CreateEditor(index).get()));
  }
}

TEST_F(VariantDependentEditorFactoryTest, NullableInteger)
{
  const VariantDependentEditorFactory factory;

  {  // item without data and without nullable flag has no editor
    auto index = AddDataToModel(variant_t{});
    m_view_model.GetSessionItemFromIndex(index)->SetData(uint32{42}, DataRole::kDefaultValue);
    EXPECT_FALSE(factory.CreateEditor(index));
  }

  {  // nullable item without data gets the type of the default value
    auto index = AddDataToModel(variant_t{});
    auto item = m_view_model.GetSessionItemFromIndex(index);
    item->SetFlag(Appearance::kNullable, true);
    item->SetData(uint32{42}, DataRole::kDefaultValue);

    auto editor = factory.CreateEditor(index);
    auto int_editor = dynamic_cast<AllIntSpinBox*>(editor.get());
    ASSERT_NE(int_editor, nullptr);
    EXPECT_TRUE(int_editor->GetLimitedInteger()->IsNullable());
    EXPECT_EQ(int_editor->GetLimitedInteger()->GetValueAsVariant(), variant_t{});
    EXPECT_EQ(int_editor->GetPlaceholderText(), QString("default"));

    EXPECT_TRUE(dynamic_cast<AllIntSpinBox*>(factory.CreateEditor(item).get()));
  }

  {  // nullable int32 with data
    auto index = AddDataToModel(variant_t{42});
    auto item = m_view_model.GetSessionItemFromIndex(index);
    item->SetFlag(Appearance::kNullable, true);
    item->SetData(int32{0}, DataRole::kDefaultValue);

    EXPECT_TRUE(dynamic_cast<AllIntSpinBox*>(factory.CreateEditor(index).get()));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBox*>(factory.CreateEditor(item).get()));
  }
}

TEST_F(VariantDependentEditorFactoryTest, NullableFloat)
{
  const VariantDependentEditorFactory factory;

  {  // nullable item without data gets the type of the default value
    auto index = AddDataToModel(variant_t{});
    auto item = m_view_model.GetSessionItemFromIndex(index);
    item->SetFlag(Appearance::kNullable, true);
    item->SetData(float32{42.0}, DataRole::kDefaultValue);

    auto editor = factory.CreateEditor(index);
    auto float_editor = dynamic_cast<ScientificSpinBox*>(editor.get());
    ASSERT_NE(float_editor, nullptr);
    EXPECT_TRUE(float_editor->GetLimitedFloat()->IsNullable());
    EXPECT_EQ(float_editor->GetLimitedFloat()->GetValueAsVariant(), variant_t{});
    EXPECT_EQ(float_editor->GetPlaceholderText(), QString("default"));

    // the step comes from the default value
    EXPECT_DOUBLE_EQ(float_editor->GetSingleStep(), 0.42);

    EXPECT_TRUE(dynamic_cast<ScientificSpinBox*>(factory.CreateEditor(item).get()));
  }

  {  // nullable item with data
    auto index = AddDataToModel(variant_t{4.2});
    auto item = m_view_model.GetSessionItemFromIndex(index);
    item->SetFlag(Appearance::kNullable, true);
    item->SetData(0.0, DataRole::kDefaultValue);

    auto editor = factory.CreateEditor(index);
    auto float_editor = dynamic_cast<ScientificSpinBox*>(editor.get());
    ASSERT_NE(float_editor, nullptr);
    EXPECT_TRUE(float_editor->GetLimitedFloat()->IsNullable());
  }
}

TEST_F(VariantDependentEditorFactoryTest, NullableBool)
{
  const VariantDependentEditorFactory factory;

  {  // nullable item without data
    auto index = AddDataToModel(variant_t{});
    auto item = m_view_model.GetSessionItemFromIndex(index);
    item->SetFlag(Appearance::kNullable, true);
    item->SetData(false, DataRole::kDefaultValue);
    item->SetData(std::string("auto"), DataRole::kPlaceholder);

    auto editor = factory.CreateEditor(index);
    auto bool_editor = dynamic_cast<NullableBoolEditor*>(editor.get());
    ASSERT_NE(bool_editor, nullptr);
    EXPECT_EQ(bool_editor->GetPlaceholderText(), QString("auto"));
  }

  {  // nullable item with data
    auto index = AddDataToModel(variant_t{true});
    auto item = m_view_model.GetSessionItemFromIndex(index);
    item->SetFlag(Appearance::kNullable, true);
    item->SetData(false, DataRole::kDefaultValue);

    EXPECT_TRUE(dynamic_cast<NullableBoolEditor*>(factory.CreateEditor(index).get()));
  }
}

}  // namespace mvvm::test
