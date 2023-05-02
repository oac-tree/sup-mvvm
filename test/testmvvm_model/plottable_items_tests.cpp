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

#include "mvvm/standarditems/plottable_items.h"

#include <gtest/gtest.h>

#include <QColor>

using namespace mvvm;

class PlottableItemsTests : public ::testing::Test
{
};

TEST_F(PlottableItemsTests, TextItemInitialState)
{
  TextItem item;
  EXPECT_TRUE(item.GetText().empty());
  EXPECT_EQ(item.GetFont(), std::string("Noto Sans"));
  EXPECT_EQ(item.GetSize(), 10);
}

TEST_F(PlottableItemsTests, TextItemGetSet)
{
  TextItem item;

  item.SetText("abc");
  EXPECT_EQ(item.GetText(), std::string("abc"));

  item.SetFont("def");
  EXPECT_EQ(item.GetFont(), std::string("def"));

  item.SetSize(42);
  EXPECT_EQ(item.GetSize(), 42);
}

TEST_F(PlottableItemsTests, PenItemInitialState)
{
  PenItem item;
  EXPECT_EQ(item.GetNamedColor(), std::string("black"));
  EXPECT_EQ(item.GetWidth(), 1);
  EXPECT_EQ(item.GetStyle(), std::string("SolidLine"));
}

TEST_F(PlottableItemsTests, PenItemSetters)
{
  PenItem item;

  item.SetSelected(true);
  EXPECT_TRUE(item.IsSelected());
  const int qt_index_dash_line = 2;  // Qt::DashLine
  EXPECT_EQ(item.Property<ComboProperty>(PenItem::kStyle).GetCurrentIndex(), qt_index_dash_line);

  const int qt_index_solid_line = 1;  // Qt::SolidLine
  item.SetSelected(false);
  EXPECT_EQ(item.Property<ComboProperty>(PenItem::kStyle).GetCurrentIndex(), qt_index_solid_line);

  item.SetNamedColor("mediumaquamarine");
  EXPECT_EQ(item.GetNamedColor(), std::string("mediumaquamarine"));

  item.SetWidth(42);
  EXPECT_EQ(item.GetWidth(), 42);
}
