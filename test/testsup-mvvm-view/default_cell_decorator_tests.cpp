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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/views/default_cell_decorator.h"

#include <mvvm/model/application_model.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>

#include <gtest/gtest.h>
#include <testutils/test_viewmodel_helper.h>

#include <QStyleOptionViewItem>

using namespace mvvm;

/**
 * @brief Tests for DefaultCellDecorator class.
 */
class DefaultCellDecoratorTest : public ::testing::Test
{
public:
  DefaultCellDecoratorTest() : m_view_model(&m_model) {}

  class TestDecorator : public DefaultCellDecorator
  {
  public:
    std::string GetText(const QModelIndex& index) const
    {
      return GetCellText(index).value_or(std::string());
    }
  };

  QModelIndex AddDataToModel(const variant_t& data)
  {
    return test::AddDataToModel(data, {}, m_view_model);
  }

  ApplicationModel m_model;
  AllItemsViewModel m_view_model;
};

TEST_F(DefaultCellDecoratorTest, BoolDecorations)
{
  const TestDecorator decorator;

  auto index = AddDataToModel(variant_t(true));
  EXPECT_TRUE(decorator.HasCustomDecoration(index));
  EXPECT_EQ(decorator.GetText(index), std::string("True"));

  index = AddDataToModel(variant_t(false));
  EXPECT_TRUE(decorator.HasCustomDecoration(index));
  EXPECT_EQ(decorator.GetText(index), std::string("False"));
}

TEST_F(DefaultCellDecoratorTest, ComboPropertyDecorations)
{
  const TestDecorator decorator;

  auto index = AddDataToModel(ComboProperty({"a1", "a2"}));
  EXPECT_TRUE(decorator.HasCustomDecoration(index));
  EXPECT_EQ(decorator.GetText(index), std::string("a1"));
}

TEST_F(DefaultCellDecoratorTest, ExternalPropertyDecorations)
{
  const TestDecorator decorator;

  auto index = AddDataToModel(ExternalProperty("text", "color"));
  EXPECT_TRUE(decorator.HasCustomDecoration(index));
  EXPECT_EQ(decorator.GetText(index), std::string("text"));
}

TEST_F(DefaultCellDecoratorTest, DoubleDecorations)
{
  const TestDecorator decorator;

  auto index = AddDataToModel(42.1234321);
  EXPECT_TRUE(decorator.HasCustomDecoration(index));
  EXPECT_EQ(decorator.GetText(index), std::string("42.1234"));
}

TEST_F(DefaultCellDecoratorTest, Int8Decoration)
{
  const TestDecorator decorator;

  {
    const mvvm::int8 value{-42};
    auto index = AddDataToModel(value);
    EXPECT_TRUE(decorator.HasCustomDecoration(index));
    EXPECT_EQ(decorator.GetText(index), std::string("-42"));
  }

  {
    const mvvm::uint8 value{42};
    auto index = AddDataToModel(value);
    EXPECT_TRUE(decorator.HasCustomDecoration(index));
    EXPECT_EQ(decorator.GetText(index), std::string("42"));
  }

  {
    const mvvm::char8 value{42};
    auto index = AddDataToModel(value);
    EXPECT_TRUE(decorator.HasCustomDecoration(index));
    EXPECT_EQ(decorator.GetText(index), std::string("42"));
  }
}

//! Variants that do not nave special decorations
TEST_F(DefaultCellDecoratorTest, DefaultDecorations)
{
  const TestDecorator decorator;

  auto index = AddDataToModel(42);
  EXPECT_FALSE(decorator.HasCustomDecoration(index));

  index = AddDataToModel(std::string("abc"));
  EXPECT_FALSE(decorator.HasCustomDecoration(index));
}

//! For own education, how QStyleOptionViewItem flags are working.
TEST_F(DefaultCellDecoratorTest, StyleOptionFlags)
{
  QStyleOptionViewItem item;

  EXPECT_EQ(item.features, QStyleOptionViewItem::ViewItemFeatures(QStyleOptionViewItem::None));

  item.features.setFlag(QStyleOptionViewItem::HasDisplay);
  EXPECT_TRUE(item.features.testFlag(QStyleOptionViewItem::HasDisplay));

  // old school way of removing the flag
  item.features &= ~QStyleOptionViewItem::HasDisplay;
  EXPECT_FALSE(item.features.testFlag(QStyleOptionViewItem::HasDisplay));

  // better way of removing flags
  item.features.setFlag(QStyleOptionViewItem::HasDisplay);
  EXPECT_TRUE(item.features.testFlag(QStyleOptionViewItem::HasDisplay));
  item.features.setFlag(QStyleOptionViewItem::HasDisplay, false);
  EXPECT_FALSE(item.features.testFlag(QStyleOptionViewItem::HasDisplay));
}

TEST_F(DefaultCellDecoratorTest, InitStyleOption)
{
  TestDecorator decorator;

  {
    QStyleOptionViewItem style_item;
    style_item.features.setFlag(QStyleOptionViewItem::HasDisplay);

    const mvvm::char8 value{42};
    auto index = AddDataToModel(value);

    decorator.InitStyleOption(index, &style_item);
    EXPECT_TRUE(style_item.features.testFlag(QStyleOptionViewItem::HasDisplay));
  }

  {
    // Special case when no text exist, only pixmap. Need to check if text's HasDisplay option is
    // removed
    ExternalProperty external_property;

    QStyleOptionViewItem style_item;
    style_item.features.setFlag(QStyleOptionViewItem::HasDisplay);

    auto index = AddDataToModel(external_property);

    decorator.InitStyleOption(index, &style_item);
    EXPECT_FALSE(style_item.features.testFlag(QStyleOptionViewItem::HasDisplay));
  }
}
