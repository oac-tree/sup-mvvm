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

#include "mvvm/plotting/customplot/axis_title_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/plottable_items.h>
#include <mvvm/standarditems/standard_item_helper.h>

#include <gtest/gtest.h>
#include <qcustomplot.h>

#include <QFont>

using namespace mvvm;

/**
 * @brief Tests for AxisTitleController class.
 */
class AxisTitleControllerTest : public ::testing::Test
{
};

TEST_F(AxisTitleControllerTest, InitialState)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  auto axis = custom_plot->xAxis;

  const AxisTitleController controller(axis);
  EXPECT_EQ(controller.GetItem(), nullptr);

  EXPECT_EQ(axis->label(), QString());
}

TEST_F(AxisTitleControllerTest, SetTextItem)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  ApplicationModel model;
  auto textItem = model.InsertItem<TextItem>();

  auto axis = custom_plot->xAxis;

  AxisTitleController controller(axis);
  controller.SetItem(textItem);
  EXPECT_EQ(controller.GetItem(), textItem);

  EXPECT_EQ(axis->label(), QString());
  EXPECT_EQ(axis->labelFont().family().toStdString(), GetDefaultAxisFontFamily());
  EXPECT_EQ(axis->labelFont().pointSize(), GetDefaultAxisTitlePointSize());
}

TEST_F(AxisTitleControllerTest, SetFont)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  ApplicationModel model;
  auto textItem = model.InsertItem<TextItem>();

  auto axis = custom_plot->xAxis;

  AxisTitleController controller(axis);
  controller.SetItem(textItem);
  EXPECT_EQ(controller.GetItem(), textItem);

  // setting new label
  const QString expected_text("abc");
  const QString expected_family("Helvetica");
  const int expected_size = 42;
  textItem->SetText(expected_text.toStdString());
  textItem->SetPointSize(expected_size);
  textItem->SetFont(expected_family.toStdString());

  // checking that label has updated
  EXPECT_EQ(axis->label(), expected_text);
  EXPECT_EQ(axis->labelFont().family(), expected_family);
  EXPECT_EQ(axis->labelFont().pointSize(), expected_size);
}
