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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/custom_editor_factories.h"

#include <mvvm/editors/custom_editor_includes.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/item_limits_helper.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>

#include <QSpinBox>

using namespace mvvm;

/**
 * @brief Tests for editor factories from custom_editor_factories header.
 */
class CustomEditorFactoriesTest : public ::testing::Test
{
public:
  CustomEditorFactoriesTest() : m_view_model(&m_model) {}

  /**
   * @brief Convenience function to add given data to the model as PropertyItem.
   *
   * @param data The data to store as data role.
   * @param editor_type Optional editor name to define for the given cell.
   * @return Index of cell which can be used to access given data.
   */
  QModelIndex AddDataToModel(const variant_t& data, const std::string& editor_type = {})
  {
    // creating item in a model and setting data to it
    auto item = m_model.InsertItem<PropertyItem>();
    item->SetData(data);
    if (!editor_type.empty())
    {
      item->SetEditorType(editor_type);
    }

    // at this point ViewModel was automatically updated, column = 1 is a cell looking to our data
    auto indexes = m_view_model.GetIndexOfSessionItem(item);
    return indexes.back();  // it should be two indices, the second is looking to our data
  }

  ApplicationModel m_model;
  AllItemsViewModel m_view_model;
};

TEST_F(CustomEditorFactoriesTest, VariantDependentEditorFactory)
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
  EXPECT_TRUE(dynamic_cast<QSpinBox*>(factory.CreateEditor(index).get()));

  {  // uint32
    const uint32 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // int64
    const int64 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // uint64
    const uint64 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // float32
    const float32 num(42.1);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<FloatSpinBox*>(factory.CreateEditor(index).get()));
  }

  {  // float64
    const float64 num(42.1);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<FloatSpinBox*>(factory.CreateEditor(index).get()));
  }
}

TEST_F(CustomEditorFactoriesTest, DefaultEditorFactory)
{
  const DefaultEditorFactory factory;

  // editor for bool types
  auto index = AddDataToModel(variant_t(true));
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(factory.CreateEditor(index).get()));

  // ComboProperty
  index = AddDataToModel(variant_t(ComboProperty()));
  EXPECT_TRUE(dynamic_cast<ComboPropertyEditor*>(factory.CreateEditor(index).get()));

  // `string` doesn't have custom editor for the moment (handled by default delegate)
  index = AddDataToModel(std::string("abc"));
  EXPECT_FALSE(factory.CreateEditor(index));

  // `string` with specification that it is a color
  index = AddDataToModel(std::string("abc"), constants::kColorEditorType);
  EXPECT_TRUE(dynamic_cast<ColorEditor*>(factory.CreateEditor(index).get()));

  {  // int32 is handled for the moment by original QSpinBox editor
    index = AddDataToModel(variant_t(42));
    EXPECT_TRUE(dynamic_cast<QSpinBox*>(factory.CreateEditor(index).get()));
  }

  {  // char8
    const char8 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // int8
    const int8 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // uint8
    const uint8 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // int16
    const int8 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // uint16
    const uint8 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // uint32
    const uint32 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // int64
    const int64 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // uint64
    const uint64 num(42);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<AllIntSpinBoxEditor*>(factory.CreateEditor(index).get()));
  }

  {  // float32
    const float32 num(42.1);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<FloatSpinBox*>(factory.CreateEditor(index).get()));
  }

  {  // float64
    const float64 num(42.1);
    index = AddDataToModel(variant_t(num));
    EXPECT_TRUE(dynamic_cast<FloatSpinBox*>(factory.CreateEditor(index).get()));
  }
}

//! Checking integer editor construction when limits are not set.
TEST_F(CustomEditorFactoriesTest, DefaultEditorFactoryIntEditor)
{
  const DefaultEditorFactory factory;

  auto index = AddDataToModel(variant_t(42));
  auto editor = factory.CreateEditor(index);

  // accessing underlying QSpinBox
  auto spin_box = dynamic_cast<QSpinBox*>(editor.get());
  ASSERT_TRUE(spin_box != nullptr);

  // checking default limits (defined in editor_builders.cpp)
  EXPECT_EQ(spin_box->minimum(), std::numeric_limits<int32>::min());
  EXPECT_EQ(spin_box->maximum(), std::numeric_limits<int32>::max());
}

//! Checking integer editor construction when limits are set.
TEST_F(CustomEditorFactoriesTest, DefaultEditorFactoryIntEditorForLimits)
{
  const DefaultEditorFactory factory;

  auto index = AddDataToModel(variant_t(42));

  // setting limits to corresponding role
  auto item = m_view_model.GetSessionItemFromIndex(index);

  SetLimited(0, 10, *item);

  auto editor = factory.CreateEditor(index);

  // accessing underlying QSpinBox
  auto spin_box = dynamic_cast<QSpinBox*>(editor.get());
  ASSERT_TRUE(spin_box != nullptr);

  // check if limits have been propagated
  EXPECT_EQ(spin_box->minimum(), 0);
  EXPECT_EQ(spin_box->maximum(), 10);
}

//! Checking double editor construction when limits are not set.
TEST_F(CustomEditorFactoriesTest, DefaultEditorFactoryDoubleEditor)
{
  const DefaultEditorFactory factory;

  auto index = AddDataToModel(variant_t(42.1));
  auto editor = factory.CreateEditor(index);

  auto spin_box = dynamic_cast<FloatSpinBox*>(editor.get());
  ASSERT_TRUE(spin_box != nullptr);

  // checking default limits (defined in editor_builders.cpp)
  EXPECT_DOUBLE_EQ(spin_box->minimum(), std::numeric_limits<double>::lowest());
  EXPECT_DOUBLE_EQ(spin_box->maximum(), std::numeric_limits<double>::max());
}

//! Checking double editor construction when limits are set.
TEST_F(CustomEditorFactoriesTest, DefaultEditorFactoryRealEditorForLimits)
{
  const DefaultEditorFactory factory;

  auto index = AddDataToModel(variant_t(42.1));

  // setting limits to corresponding role
  auto item = m_view_model.GetSessionItemFromIndex(index);
  SetLimited(0.0, 55.1, *item);

  auto editor = factory.CreateEditor(index);

  // accessing underlying QSpinBox
  auto spin_box = dynamic_cast<FloatSpinBox*>(editor.get());
  ASSERT_TRUE(spin_box != nullptr);

  // check if limits have been propagated
  EXPECT_DOUBLE_EQ(spin_box->minimum(), 0);
  EXPECT_DOUBLE_EQ(spin_box->maximum(), 55.1);
}

//! Checking double editor construction when lower limit is set.
TEST_F(CustomEditorFactoriesTest, DefaultEditorFactoryRealEditorForLowerLimited)
{
  const DefaultEditorFactory factory;

  auto index = AddDataToModel(variant_t(42.1));

  // setting limits to corresponding role
  auto item = m_view_model.GetSessionItemFromIndex(index);
  SetLowerLimited(42.0, *item);

  auto editor = factory.CreateEditor(index);

  // accessing underlying QSpinBox
  auto spin_box = dynamic_cast<FloatSpinBox*>(editor.get());
  ASSERT_TRUE(spin_box != nullptr);

  // check if limits have been propagated
  EXPECT_DOUBLE_EQ(spin_box->minimum(), 42.0);
  EXPECT_DOUBLE_EQ(spin_box->maximum(), std::numeric_limits<double>::max());
}
