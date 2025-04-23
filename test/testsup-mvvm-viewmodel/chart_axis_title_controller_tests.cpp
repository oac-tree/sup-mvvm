/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "mvvm/plotting/charts/chart_axis_title_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/plotting/charts/qt_charts.h>
#include <mvvm/standarditems/plottable_items.h>

#include <gtest/gtest.h>

using namespace mvvm;

/**
 * @brief Tests for ChartAxisTitleController class.
 */
class ChartAxisTitleControllerTest : public ::testing::Test
{
};

TEST_F(ChartAxisTitleControllerTest, InitialState)
{
  QValueAxis axis;

  EXPECT_THROW(ChartAxisTitleController(nullptr), RuntimeException);

  mvvm::ApplicationModel model;
  auto text_item = model.InsertItem<mvvm::TextItem>();

  ChartAxisTitleController controller(&axis);
  EXPECT_EQ(controller.GetQtAxis(), &axis);
  EXPECT_EQ(controller.GetItem(), nullptr);

  controller.SetItem(text_item);
  EXPECT_EQ(controller.GetItem(), text_item);

  EXPECT_EQ(axis.titleText(), QString());
}

TEST_F(ChartAxisTitleControllerTest, SetTextItem)
{
  QValueAxis axis;

  EXPECT_THROW(ChartAxisTitleController(nullptr), RuntimeException);

  mvvm::ApplicationModel model;
  auto text_item = model.InsertItem<mvvm::TextItem>();
  text_item->SetPointSize(42);
  text_item->SetText("abc");

  ChartAxisTitleController controller(&axis);

  controller.SetItem(text_item);

  EXPECT_EQ(controller.GetQtAxis(), &axis);
  EXPECT_EQ(controller.GetItem(), text_item);

  EXPECT_EQ(axis.titleText(), QString("abc"));
  EXPECT_EQ(axis.titleFont().pointSize(), 42);
}

TEST_F(ChartAxisTitleControllerTest, SetFont)
{
  QValueAxis axis;

  EXPECT_THROW(ChartAxisTitleController(nullptr), RuntimeException);

  mvvm::ApplicationModel model;
  auto text_item = model.InsertItem<mvvm::TextItem>();
  text_item->SetPointSize(42);
  text_item->SetText("abc");

  ChartAxisTitleController controller(&axis);

  controller.SetItem(text_item);

  // setting new label
  const QString expected_text("abc");
  const QString expected_family("Helvetica");
  const int expected_size = 42;
  text_item->SetText(expected_text.toStdString());
  text_item->SetPointSize(expected_size);
  text_item->SetFont(expected_family.toStdString());

  // checking that label has updated
  EXPECT_EQ(axis.titleText(), expected_text);
  EXPECT_EQ(axis.titleFont().family(), expected_family);
  EXPECT_EQ(axis.titleFont().pointSize(), expected_size);
}
