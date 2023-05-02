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

#include "mvvm/standarditems/data1d_item.h"

#include <mvvm/model/sessionmodel.h>
#include <mvvm/standarditems/axis_items.h>

#include <gtest/gtest.h>

#include <stdexcept>

using namespace mvvm;

//! Testing Data1DItem.

class Data1DItemTests : public ::testing::Test
{
};

//! Initial state.

TEST_F(Data1DItemTests, InitialState)
{
  Data1DItem item;

  EXPECT_EQ(item.GetAxis(), nullptr);
  EXPECT_EQ(item.GetBinCenters(), std::vector<double>());
  EXPECT_EQ(item.GetValues(), std::vector<double>());
  EXPECT_EQ(item.GetErrors(), std::vector<double>());
  EXPECT_FALSE(item.HasData());
}

//! Checking the method ::setFixedBinAxis.

TEST_F(Data1DItemTests, SetFixedBinAxis)
{
  Data1DItem item;

  item.SetAxis<FixedBinAxisItem>(5, 0.0, 5.0);

  // check type of the axis
  EXPECT_TRUE(item.GetAxis() != nullptr);

  // check bin centers and values
  std::vector<double> expected_centers = {0.5, 1.5, 2.5, 3.5, 4.5};
  EXPECT_EQ(item.GetBinCenters(), expected_centers);
  std::vector<double> expected_values = std::vector<double>(expected_centers.size(), 0.0);
  EXPECT_EQ(item.GetValues(), expected_values);

  // setting another axis
  // for the moment we have disabled possibility to re-create axes to faciltate undo/redo
  EXPECT_THROW(item.SetAxis(FixedBinAxisItem::Create(1, 1.0, 2.0)), std::runtime_error);
}

//! Sets fixed bin axis via templated method.

TEST_F(Data1DItemTests, SetTemplatedFixedBinAxis)
{
  Data1DItem item;

  auto axis = item.SetAxis<FixedBinAxisItem>(5, 0.0, 5.0);

  // check type of the axis
  EXPECT_EQ(item.GetAxis(), axis);

  // check bin centers and values
  std::vector<double> expected_centers = {0.5, 1.5, 2.5, 3.5, 4.5};
  EXPECT_EQ(item.GetBinCenters(), expected_centers);
  std::vector<double> expected_values = std::vector<double>(expected_centers.size(), 0.0);
  EXPECT_EQ(item.GetValues(), expected_values);
}

//! Sets fixed bin axis via templated method.

TEST_F(Data1DItemTests, SetTemplatedFixedBinAxisInModelContext)
{
  SessionModel model;
  auto dataItem = model.InsertItem<Data1DItem>();

  auto axis = dataItem->SetAxis<FixedBinAxisItem>(5, 0.0, 5.0);

  // check type of the axis
  EXPECT_EQ(dataItem->GetAxis(), axis);

  // check bin centers and values
  std::vector<double> expected_centers = {0.5, 1.5, 2.5, 3.5, 4.5};
  EXPECT_EQ(dataItem->GetBinCenters(), expected_centers);
  std::vector<double> expected_values = std::vector<double>(expected_centers.size(), 0.0);
  EXPECT_EQ(dataItem->GetValues(), expected_values);
}

//! Sets fixed bin axis via model context.
// FIXME Not clear if this method should be used

TEST_F(Data1DItemTests, SetFixedBinAxisInModel)
{
  SessionModel model;

  auto dataItem = model.InsertItem<Data1DItem>();
  model.InsertItem<FixedBinAxisItem>(dataItem)->SetParameters(5, 0.0, 5.0);

  // check type of the axis
  EXPECT_TRUE(dataItem->GetAxis() != nullptr);

  // check bin centers and values
  std::vector<double> expected_centers = {0.5, 1.5, 2.5, 3.5, 4.5};
  EXPECT_EQ(dataItem->GetBinCenters(), expected_centers);
  std::vector<double> expected_values = std::vector<double>(expected_centers.size(), 0.0);
  EXPECT_TRUE(dataItem->GetValues().empty());
}

//! Checking the method ::setValues.

TEST_F(Data1DItemTests, SetValues)
{
  Data1DItem item;

  // check that it is not possible to set content to uninitialized axis
  std::vector<double> expected_content = {1.0, 2.0, 3.0};
  EXPECT_THROW(item.SetValues(expected_content), std::runtime_error);

  item.SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  item.SetValues(expected_content);
  EXPECT_EQ(item.GetValues(), expected_content);
}

//! Checking the method ::setErrors.

TEST_F(Data1DItemTests, SetErrors)
{
  Data1DItem item;

  // check that it is not possible to errors to uninitialized axis
  std::vector<double> expected_errors = {10.0, 20.0, 30.0};

  EXPECT_THROW(item.SetErrors(expected_errors), std::runtime_error);

  item.SetAxis<FixedBinAxisItem>(3, 0.0, 3.0);
  item.SetErrors(expected_errors);

  EXPECT_EQ(item.GetErrors(), expected_errors);
}

//! Checking the signals when axes changed.
//! FIXME enable tests checkSignalsOnAxisChange

// TEST_F(Data1DItemTests, checkSignalsOnAxisChange)
//{
//     SessionModel model;
//     auto item = model.insertItem<Data1DItem>();

//    MockWidgetForItem widget(item);

//    const std::string expected_value_tag{Data1DItem::P_VALUES};
//    const TagRow expected_axis_tagrow{Data1DItem::T_AXIS, 0};

//    EXPECT_CALL(widget, onDataChange(_, _)).Times(0);
//    EXPECT_CALL(widget, onPropertyChange(item, expected_value_tag)).Times(1);
//    EXPECT_CALL(widget, onChildPropertyChange(_, _)).Times(2);
//    EXPECT_CALL(widget, onItemInserted(item, expected_axis_tagrow)).Times(1);
//    EXPECT_CALL(widget, onAboutToRemoveItem(_, _)).Times(0);

//    // trigger change
//    item->setAxis<FixedBinAxisItem>(3, 0.0, 3.0);
//}

//! Checking the signals when bin values changed.
//! FIXME enable tests

// TEST_F(Data1DItemTests, checkSignalsOnBinValuesChange)
//{
//     SessionModel model;
//     auto item = model.insertItem<Data1DItem>();
//     item->setAxis<FixedBinAxisItem>(3, 0.0, 3.0);

//    MockWidgetForItem widget(item);

//    EXPECT_CALL(widget, onDataChange(_, _)).Times(0);
//    EXPECT_CALL(widget, onPropertyChange(item, Data1DItem::P_VALUES)).Times(1);
//    EXPECT_CALL(widget, onChildPropertyChange(_, _)).Times(0);
//    EXPECT_CALL(widget, onItemInserted(_, _)).Times(0);
//    EXPECT_CALL(widget, onAboutToRemoveItem(_, _)).Times(0);

//    // trigger change
//    item->setValues(std::vector<double>{1.0, 2.0, 3.0});
//}

//! Testing clone

TEST_F(Data1DItemTests, Clone)
{
  SessionModel model;
  auto data_item = model.InsertItem<Data1DItem>();
  auto axis_item = data_item->SetAxis<FixedBinAxisItem>(5, 0.0, 5.0);

  auto clone = data_item->Clone(/* make_unique_id*/ false);
  auto data_clone = dynamic_cast<Data1DItem*>(clone.get());
  ASSERT_NE(data_clone, nullptr);
  auto axis_clone = data_clone->GetAxis();

  EXPECT_EQ(axis_clone->GetIdentifier(), axis_item->GetIdentifier());

  // check bin centers and values
  std::vector<double> expected_centers = {0.5, 1.5, 2.5, 3.5, 4.5};
  EXPECT_EQ(data_clone->GetBinCenters(), expected_centers);
  std::vector<double> expected_values = std::vector<double>(expected_centers.size(), 0.0);
  EXPECT_EQ(data_clone->GetValues(), expected_values);
}
