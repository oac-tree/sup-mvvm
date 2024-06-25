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

#include "mvvm/standarditems/line_series_item.h"

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/line_series_data_item.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for LineSeriesItem class.

class LineSeriesItemTest : public ::testing::Test
{
};

TEST_F(LineSeriesItemTest, LineSeriesItemInitialState)
{
  const LineSeriesItem item;
  EXPECT_TRUE(item.GetDataItem() == nullptr);
}

TEST_F(LineSeriesItemTest, LineSeriesItemSetNamedColor)
{
  LineSeriesItem item;
  item.SetNamedColor("mediumaquamarine");
  EXPECT_EQ(item.GetNamedColor(), std::string("mediumaquamarine"));
}

//! Validating link mechanism.

TEST_F(LineSeriesItemTest, LineSeriesItemSetData)
{
  mvvm::ApplicationModel model;

  // creating data item, and linking series item to it
  auto data_item = model.InsertItem<LineSeriesDataItem>();
  auto line_series_item = model.InsertItem<LineSeriesItem>();
  line_series_item->SetDataItem(data_item);

  // checking that link is working
  EXPECT_EQ(line_series_item->GetDataItem(), data_item);
}
