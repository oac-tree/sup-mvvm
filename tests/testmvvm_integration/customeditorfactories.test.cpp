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
  QModelIndex AddDataToModel(const variant_t& data)
  {
    // creating item in a model and setting data to it
    auto item = m_model.InsertItem<PropertyItem>();
    item->SetData(data);

    // at this point ViewModel was automatically updated, column = 1 is a cell looking to our data
    auto indexes = m_view_model.GetIndexOfSessionItem(item);
    return indexes.back();  // it should be two indices, the second is looking to our data
  }

  ApplicationModel m_model;
  AllItemsViewModel m_view_model;
};

TEST_F(CustomEditorFactoriesTest, VariantDependentEditorFactory)
{
  VariantDependentEditorFactory factory;

  // editor for bool types
  auto index1 = AddDataToModel(variant_t(true));
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(factory.CreateEditor(index1).get()));

  // ComboProperty
  auto index2 = AddDataToModel(variant_t(ComboProperty()));
  EXPECT_TRUE(dynamic_cast<ComboPropertyEditor*>(factory.CreateEditor(index2).get()));

  // `double` doesn't have custom editor for the moment (handled by default delegate)
  auto index3 = AddDataToModel(variant_t(42.1));
  EXPECT_FALSE(factory.CreateEditor(index3));

  // `int` doesn't have custom editor for the moment (handled by default delegate)
  auto index4 = AddDataToModel(variant_t(42));
  EXPECT_FALSE(factory.CreateEditor(index4));

  // `string` doesn't have custom editor for the moment (handled by default delegate)
  auto index5 = AddDataToModel(std::string("abc"));
  EXPECT_FALSE(factory.CreateEditor(index5));
}

//! Default editor factory for the moment reproduces VariantDependentEditorFactory

TEST_F(CustomEditorFactoriesTest, DefaultEditorFactory)
{
  DefaultEditorFactory factory;

  // editor for bool types
  auto index1 = AddDataToModel(variant_t(true));
  EXPECT_TRUE(dynamic_cast<BoolEditor*>(factory.CreateEditor(index1).get()));

  // ComboProperty
  auto index2 = AddDataToModel(variant_t(ComboProperty()));
  EXPECT_TRUE(dynamic_cast<ComboPropertyEditor*>(factory.CreateEditor(index2).get()));

  // `double` doesn't have custom editor for the moment (handled by default delegate)
  auto index3 = AddDataToModel(variant_t(42.1));
  EXPECT_FALSE(factory.CreateEditor(index3));

  // `int` doesn't have custom editor for the moment (handled by default delegate)
  auto index4 = AddDataToModel(variant_t(42));
  EXPECT_FALSE(factory.CreateEditor(index4));

  // `string` doesn't have custom editor for the moment (handled by default delegate)
  auto index5 = AddDataToModel(std::string("abc"));
  EXPECT_FALSE(factory.CreateEditor(index5));
}
