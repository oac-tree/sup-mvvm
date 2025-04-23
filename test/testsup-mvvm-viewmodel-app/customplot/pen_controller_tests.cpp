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

#include "mvvm/plotting/customplot/pen_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/standarditems/plottable_items.h>

#include <gtest/gtest.h>
#include <qcustomplot.h>

using namespace mvvm;

//! Testing PenController.

class PenControllerTest : public ::testing::Test
{
};

//! Initial state.

TEST_F(PenControllerTest, InitialState)
{
  // Constructor accept valid QCPGraph
  EXPECT_THROW(PenController(nullptr), RuntimeException);

  auto custom_plot = std::make_unique<QCustomPlot>();
  auto graph = custom_plot->addGraph();

  PenController controller(graph);
  EXPECT_EQ(controller.GetItem(), nullptr);
}

TEST_F(PenControllerTest, GraphItemInInitialState)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  auto graph = custom_plot->addGraph();

  PenController controller(graph);

  ApplicationModel model;
  auto pen_item = model.InsertItem<PenItem>();
  controller.SetItem(pen_item);

  EXPECT_EQ(controller.GetItem(), pen_item);

  // parameters of graph in QCustomPlot
  EXPECT_EQ(graph->pen().color(), QColor(Qt::black));
  EXPECT_EQ(graph->pen().style(), Qt::SolidLine);
  EXPECT_EQ(graph->pen().width(), 1);
}

TEST_F(PenControllerTest, SetPenSelected)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  auto graph = custom_plot->addGraph();

  PenController controller(graph);

  ApplicationModel model;
  auto pen_item = model.InsertItem<PenItem>();
  controller.SetItem(pen_item);

  pen_item->SetSelected(true);

  // parameters of graph in QCustomPlot
  EXPECT_EQ(graph->pen().color(), QColor(Qt::black));
  EXPECT_EQ(graph->pen().style(), Qt::DashLine);
  EXPECT_EQ(graph->pen().width(), 1);
}

TEST_F(PenControllerTest, SetColorAndWidth)
{
  auto custom_plot = std::make_unique<QCustomPlot>();
  auto graph = custom_plot->addGraph();

  PenController controller(graph);

  ApplicationModel model;
  auto pen_item = model.InsertItem<PenItem>();
  controller.SetItem(pen_item);

  pen_item->SetWidth(2);
  pen_item->SetNamedColor("red");

  // parameters of graph in QCustomPlot
  EXPECT_EQ(graph->pen().color(), QColor(Qt::red));
  EXPECT_EQ(graph->pen().style(), Qt::SolidLine);
  EXPECT_EQ(graph->pen().width(), 2);

  // set color via named color machinery
  pen_item->SetNamedColor("azure");
  EXPECT_EQ(graph->pen().color().name(), QString("#f0ffff"));
}
