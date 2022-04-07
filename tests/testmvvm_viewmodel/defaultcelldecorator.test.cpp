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

#include "mvvm/delegates/defaultcelldecorator.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/viewmodel/allitemsviewmodel.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for DefaultCellDecoratorTest.

class DefaultCellDecoratorTest : public ::testing::Test
{
public:
  DefaultCellDecoratorTest() : m_view_model(&m_model) {}

  class TestDecorator : public DefaultCellDecorator
  {
  public:
    std::string GetText(const QModelIndex& index) const { return GetCellText(index).value(); }
  };

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

// using variant_t = std::variant<std::monostate, bool, int, double, std::string,
// std::vector<double>,
//                                mvvm::ComboProperty, mvvm::ExternalProperty>;

TEST_F(DefaultCellDecoratorTest, BoolDecorations)
{
  TestDecorator decorator;

  auto index = AddDataToModel(variant_t(true));
  EXPECT_TRUE(decorator.HasCustomDecoration(index));
  EXPECT_EQ(decorator.GetText(index), std::string("True"));

  index = AddDataToModel(variant_t(false));
  EXPECT_TRUE(decorator.HasCustomDecoration(index));
  EXPECT_EQ(decorator.GetText(index), std::string("False"));
}

TEST_F(DefaultCellDecoratorTest, ComboPropertyDecorations)
{
  TestDecorator decorator;

  auto index = AddDataToModel(ComboProperty({"a1", "a2"}));
  EXPECT_TRUE(decorator.HasCustomDecoration(index));
  EXPECT_EQ(decorator.GetText(index), std::string("a1"));
}

TEST_F(DefaultCellDecoratorTest, ExternalPropertyDecorations)
{
  TestDecorator decorator;

  auto index = AddDataToModel(ExternalProperty("text", "color"));
  EXPECT_TRUE(decorator.HasCustomDecoration(index));
  EXPECT_EQ(decorator.GetText(index), std::string("text"));
}

TEST_F(DefaultCellDecoratorTest, DoubleDecorations)
{
  TestDecorator decorator;

  auto index = AddDataToModel(42.1234321);
  EXPECT_TRUE(decorator.HasCustomDecoration(index));
  EXPECT_EQ(decorator.GetText(index), std::string("42.1234"));
}

//! Variants that do not nave special decorations

TEST_F(DefaultCellDecoratorTest, DefaultDecorations)
{
  TestDecorator decorator;

  auto index = AddDataToModel(42);
  EXPECT_FALSE(decorator.HasCustomDecoration(index));

  index = AddDataToModel(std::string("abc"));
  EXPECT_FALSE(decorator.HasCustomDecoration(index));
}
