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

#include "mvvm/plotting/axistitlecontroller.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/standarditems/plottableitems.h"

#include <gtest/gtest.h>
#include <qcustomplot.h>

#include <QFont>

using namespace mvvm;

//! Testing AxisTitleControllers.

class AxisTitleControllerTest : public ::testing::Test
{
};

//! Initial state.

TEST_F(AxisTitleControllerTest, InitialState)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  auto axis = custom_plot->xAxis;

  // controller shouldn''t change axis range
  AxisTitleController controller(axis);
  EXPECT_EQ(controller.GetItem(), nullptr);

  EXPECT_EQ(axis->label(), QString());
}

TEST_F(AxisTitleControllerTest, SetTextItem)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  ApplicationModel model;
  auto textItem = model.InsertItem<TextItem>();

  auto axis = custom_plot->xAxis;
  //    auto expected_pointSize = axis->labelFont().pointSize();
  //    auto expected_family = axis->labelFont().family();

  // this a values hardcoded in plottableitems.cpp. Shell we provide some customized way to create
  // TextItem with font/size suitable for QCPAxis ?
  const int expected_pointSize = 10;
  const std::string expected_family = "Noto Sans";

  // controller shouldn''t change axis range
  AxisTitleController controller(axis);
  controller.SetItem(textItem);
  EXPECT_EQ(controller.GetItem(), textItem);

  EXPECT_EQ(axis->label(), QString());
  EXPECT_EQ(axis->labelFont().family().toStdString(), expected_family);
  EXPECT_EQ(axis->labelFont().pointSize(), expected_pointSize);
}

TEST_F(AxisTitleControllerTest, SetFont)
{
  auto custom_plot = std::make_unique<QCustomPlot>();

  ApplicationModel model;
  auto textItem = model.InsertItem<TextItem>();

  auto axis = custom_plot->xAxis;

  // controller shouldn''t change axis range
  AxisTitleController controller(axis);
  controller.SetItem(textItem);
  EXPECT_EQ(controller.GetItem(), textItem);

  // setting new label
  const QString expected_text("abc");
  const QString expected_family("Helvetica");
  const int expected_size = 42;
  textItem->SetText(expected_text.toStdString());
  textItem->SetSize(expected_size);
  textItem->SetFont(expected_family.toStdString());

  // checking that label has updated
  EXPECT_EQ(axis->label(), expected_text);
  EXPECT_EQ(axis->labelFont().family(), expected_family);
  EXPECT_EQ(axis->labelFont().pointSize(), expected_size);
}
