/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/standarditems/vectoritem.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/itemutils.h"
#include "mvvm/model/sessionmodel.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! VectorItem tests.

class VectorItemTest : public ::testing::Test
{
};

//! Initial state of item when it is created outside of model context.

TEST_F(VectorItemTest, InitialState)
{
  VectorItem item;

  EXPECT_TRUE(utils::IsSinglePropertyTag(item, VectorItem::kX));
  EXPECT_TRUE(utils::IsSinglePropertyTag(item, VectorItem::kY));
  EXPECT_TRUE(utils::IsSinglePropertyTag(item, VectorItem::kZ));

  EXPECT_FALSE(item.IsEditable());

  EXPECT_EQ(item.Property<double>(VectorItem::kX), 0.0);
  EXPECT_EQ(item.Property<double>(VectorItem::kY), 0.0);
  EXPECT_EQ(item.Property<double>(VectorItem::kZ), 0.0);

  // default label
  EXPECT_EQ(item.Data<std::string>(), "(0, 0, 0)");
}

//! Initial state of item in model context

TEST_F(VectorItemTest, InitialStateFromModel)
{
  SessionModel model;
  auto item = model.InsertItem<VectorItem>();

  EXPECT_EQ(item->Property<double>(VectorItem::kX), 0.0);
  EXPECT_EQ(item->Property<double>(VectorItem::kY), 0.0);
  EXPECT_EQ(item->Property<double>(VectorItem::kZ), 0.0);

  // default label
  EXPECT_EQ(item->Data<std::string>(), "(0, 0, 0)");
}

TEST_F(VectorItemTest, SetXYZ)
{
  SessionModel model;
  auto item = model.InsertItem<VectorItem>();
  item->SetXYZ(1.0, 2.0, 3.0);

  EXPECT_EQ(item->X(), 1.0);
  EXPECT_EQ(item->Y(), 2.0);
  EXPECT_EQ(item->Z(), 3.0);
}

//! Checking label update in ApplicationModel context

TEST_F(VectorItemTest, LabelUpdate)
{
  ApplicationModel model;
  auto item = model.InsertItem<VectorItem>();

  EXPECT_EQ(item->Data<std::string>(), "(0, 0, 0)");

  // Modification of one of the property should lead to label update
  item->SetProperty(VectorItem::kX, 1.0);

  // Updated thanks to VectorItem::Activate
  EXPECT_EQ(item->Data<std::string>(), "(1, 0, 0)");
}
