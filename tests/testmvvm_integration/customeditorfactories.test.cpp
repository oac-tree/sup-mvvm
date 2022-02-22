/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/customeditorfactories.h"

#include "mvvm/editors/customeditorincludes.h"
#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/standarditems/editor_constants.h"
#include "mvvm/viewmodel/allitemsviewmodel.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for AbstractEditorFactory.

class CustomEditorFactoriesTest : public ::testing::Test
{
public:
  CustomEditorFactoriesTest() : m_view_model(&m_model) {}

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

TEST_F(CustomEditorFactoriesTest, RoleDependentEditorFactory)
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
}

TEST_F(CustomEditorFactoriesTest, VariantDependentEditorFactory)
{
  VariantDependentEditorFactory factory;

  // editor for bool types
  auto index = AddDataToModel(variant_t(true));
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(factory.CreateEditor(index).get()));

  // ComboProperty
  index = AddDataToModel(variant_t(ComboProperty()));
  EXPECT_TRUE(dynamic_cast<ComboPropertyEditor*>(factory.CreateEditor(index).get()));

  // `double` doesn't have custom editor for the moment (handled by default delegate)
  index = AddDataToModel(variant_t(42.1));
  EXPECT_FALSE(factory.CreateEditor(index));

  // `int` doesn't have custom editor for the moment (handled by default delegate)
  index = AddDataToModel(variant_t(42));
  EXPECT_FALSE(factory.CreateEditor(index));

  // `string` doesn't have custom editor for the moment (handled by default delegate)
  index = AddDataToModel(std::string("abc"));
  EXPECT_FALSE(factory.CreateEditor(index));
}

//! Default editor factory for the moment reproduces VariantDependentEditorFactory

TEST_F(CustomEditorFactoriesTest, DefaultEditorFactory)
{
  DefaultEditorFactory factory;

  // editor for bool types
  auto index = AddDataToModel(variant_t(true));
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(factory.CreateEditor(index).get()));

  // ComboProperty
  index = AddDataToModel(variant_t(ComboProperty()));
  EXPECT_TRUE(dynamic_cast<ComboPropertyEditor*>(factory.CreateEditor(index).get()));

  // `double` doesn't have custom editor for the moment (handled by default delegate)
  index = AddDataToModel(variant_t(42.1));
  EXPECT_FALSE(factory.CreateEditor(index));

  // `int` doesn't have custom editor for the moment (handled by default delegate)
  index = AddDataToModel(variant_t(42));
  EXPECT_FALSE(factory.CreateEditor(index));

  // `string` doesn't have custom editor for the moment (handled by default delegate)
  index = AddDataToModel(std::string("abc"));
  EXPECT_FALSE(factory.CreateEditor(index));

  // `string` with specification that it is a color
  index = AddDataToModel(std::string("abc"), constants::kColorEditorType);
  EXPECT_TRUE(dynamic_cast<ColorEditor*>(factory.CreateEditor(index).get()));
}
