/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "mvvm/plotting/charts/chart_axis_title_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/plottable_items.h>

#include <gtest/gtest.h>

#include <QtCharts/QValueAxis>

using namespace mvvm;

/**
 * @brief Tests for ChartAxisTitleController class.
 */
class ChartAxisTitleControllerTest : public ::testing::Test
{
};

//! Initial axis title after controller setup.
TEST_F(ChartAxisTitleControllerTest, InitialState)
{
  QtCharts::QValueAxis axis;

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


