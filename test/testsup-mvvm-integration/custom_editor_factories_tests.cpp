/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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
#include <mvvm/model/limits.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>

#include <QSpinBox>

using namespace mvvm;

//! Tests for AbstractEditorFactory.

class CustomEditorFactoriesTests : public ::testing::Test
{
public:
  CustomEditorFactoriesTests() : m_view_model(&m_model) {}

  //! Convenience function to add given data to the model as PropertyItem.
  //! Returns back an index corresponding to the item's position in a view model.
  //! Given index will be used to create a corresponding cell editor using one of the editor
  //! factories.
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

TEST_F(CustomEditorFactoriesTests, RoleDependentEditorFactory)
{
  RoleDependentEditorFactory factory;

  // editor for bool types
  auto index = AddDataToModel(variant_t(true), constants::kBoolEditorType);
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(factory.CreateEditor(index).get()));

  // ComboProperty
  index = AddDataToModel(variant_t(ComboProperty()), constants::kComboPropertyEditorType);
  EXPECT_TRUE(dynamic_cast<ComboPropertyEditor*>(factory.CreateEditor(index).get()));

  // ComboProperty
  index = AddDataToModel(variant_t(ComboProperty()), constants::kSelectableComboPropertyEditorType);
  EXPECT_TRUE(dynamic_cast<SelectableComboBoxEditor*>(factory.CreateEditor(index).get()));

  // String as color
  index = AddDataToModel(variant_t("red"), constants::kColorEditorType);
  EXPECT_TRUE(dynamic_cast<ColorEditor*>(factory.CreateEditor(index).get()));

  // double
  index = AddDataToModel(variant_t(42.2), constants::kScientificSpinboxEditorType);
  EXPECT_TRUE(dynamic_cast<ScientificSpinBoxEditor*>(factory.CreateEditor(index).get()));

  // long int
  index = AddDataToModel(variant_t(4294967296LL), constants::kLongIntSpinBoxEditorType);
  EXPECT_TRUE(dynamic_cast<LongIntSpinBoxEditor*>(factory.CreateEditor(index).get()));

  // for int we use adapted QSpinBox with limits set
  index = AddDataToModel(variant_t(42), constants::kIntegerEditorType);
  EXPECT_TRUE(dynamic_cast<QSpinBox*>(factory.CreateEditor(index).get()));
}

TEST_F(CustomEditorFactoriesTests, VariantDependentEditorFactory)
{
  VariantDependentEditorFactory factory;

  // editor for bool types
  auto index = AddDataToModel(variant_t(true));
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(factory.CreateEditor(index).get()));

  // ComboProperty
  index = AddDataToModel(variant_t(ComboProperty()));
  EXPECT_TRUE(dynamic_cast<ComboPropertyEditor*>(factory.CreateEditor(index).get()));

  // `string` doesn't have custom editor for the moment (handled by default delegate)
  index = AddDataToModel(std::string("abc"));
  EXPECT_FALSE(factory.CreateEditor(index));

  // int
  index = AddDataToModel(variant_t(42));
  EXPECT_TRUE(dynamic_cast<QSpinBox*>(factory.CreateEditor(index).get()));

  // double
  index = AddDataToModel(variant_t(42.1));
  EXPECT_TRUE(dynamic_cast<QDoubleSpinBox*>(factory.CreateEditor(index).get()));
}

TEST_F(CustomEditorFactoriesTests, DefaultEditorFactory)
{
  DefaultEditorFactory factory;

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

  // editor for int types
  index = AddDataToModel(variant_t(42));
  EXPECT_TRUE(dynamic_cast<QSpinBox*>(factory.CreateEditor(index).get()));

  // double
  index = AddDataToModel(variant_t(42.1));
  EXPECT_TRUE(dynamic_cast<QDoubleSpinBox*>(factory.CreateEditor(index).get()));
}

//! Checking integer editor construction when limits are not set.

TEST_F(CustomEditorFactoriesTests, DefaultEditorFactoryIntEditor)
{
  DefaultEditorFactory factory;

  auto index = AddDataToModel(variant_t(42));
  auto editor = factory.CreateEditor(index);

  // accessing underlying QSpinBox
  auto spin_box = dynamic_cast<QSpinBox*>(editor.get());
  ASSERT_TRUE(spin_box != nullptr);

  // checking default limits (defined in editor_builders.cpp)
  EXPECT_EQ(spin_box->minimum(), -65536);
  EXPECT_EQ(spin_box->maximum(), 65536);
}

//! Checking integer editor construction when limits are set.

TEST_F(CustomEditorFactoriesTests, DefaultEditorFactoryIntEditorForLimits)
{
  DefaultEditorFactory factory;

  auto index = AddDataToModel(variant_t(42));

  // setting limits to corresponding role
  auto item = const_cast<SessionItem*>(m_view_model.GetSessionItemFromIndex(index));
  item->SetData(Limits<int>::CreateLimited(0, 10), DataRole::kLimits);

  auto editor = factory.CreateEditor(index);

  // accessing underlying QSpinBox
  auto spin_box = dynamic_cast<QSpinBox*>(editor.get());
  ASSERT_TRUE(spin_box != nullptr);

  // check if limits have been propagated
  EXPECT_EQ(spin_box->minimum(), 0);
  EXPECT_EQ(spin_box->maximum(), 10);
}

//! Checking double editor construction when limits are not set.

TEST_F(CustomEditorFactoriesTests, DefaultEditorFactoryDoubleEditor)
{
  DefaultEditorFactory factory;

  auto index = AddDataToModel(variant_t(42.1));
  auto editor = factory.CreateEditor(index);

  // accessing underlying QDoubleSpinBox
  auto spin_box = dynamic_cast<QDoubleSpinBox*>(editor.get());
  ASSERT_TRUE(spin_box != nullptr);

  // checking default limits (defined in editor_builders.cpp)
  EXPECT_DOUBLE_EQ(spin_box->minimum(), -65536);
  EXPECT_DOUBLE_EQ(spin_box->maximum(), 65536);
}

//! Checking integer editor construction when limits are set.

TEST_F(CustomEditorFactoriesTests, DefaultEditorFactoryRealEditorForLimits)
{
  DefaultEditorFactory factory;

  auto index = AddDataToModel(variant_t(42.1));

  // setting limits to corresponding role
  auto item = const_cast<SessionItem*>(m_view_model.GetSessionItemFromIndex(index));
  item->SetData(Limits<double>::CreateLimited(0.0, 55.1), DataRole::kLimits);

  auto editor = factory.CreateEditor(index);

  // accessing underlying QSpinBox
  auto spin_box = dynamic_cast<QDoubleSpinBox*>(editor.get());
  ASSERT_TRUE(spin_box != nullptr);

  // check if limits have been propagated
  EXPECT_DOUBLE_EQ(spin_box->minimum(), 0);
  EXPECT_DOUBLE_EQ(spin_box->maximum(), 55.1);
}
