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

  EXPECT_TRUE(utils::IsSinglePropertyTag(item, VectorItem::P_X));
  EXPECT_TRUE(utils::IsSinglePropertyTag(item, VectorItem::P_Y));
  EXPECT_TRUE(utils::IsSinglePropertyTag(item, VectorItem::P_Z));

  EXPECT_FALSE(item.IsEditable());

  EXPECT_EQ(item.Property<double>(VectorItem::P_X), 0.0);
  EXPECT_EQ(item.Property<double>(VectorItem::P_Y), 0.0);
  EXPECT_EQ(item.Property<double>(VectorItem::P_Z), 0.0);

  // default label
  EXPECT_EQ(item.Data<std::string>(), "(0, 0, 0)");
}

//! Initial state of item in model context

TEST_F(VectorItemTest, InitialStateFromModel)
{
  SessionModel model;
  auto item = model.InsertItem<VectorItem>();

  EXPECT_EQ(item->Property<double>(VectorItem::P_X), 0.0);
  EXPECT_EQ(item->Property<double>(VectorItem::P_Y), 0.0);
  EXPECT_EQ(item->Property<double>(VectorItem::P_Z), 0.0);

  // default label
  EXPECT_EQ(item->Data<std::string>(), "(0, 0, 0)");

  // FIXME uncomment. Will work only after restoring callbacks
  // changing vector component
  //  item->setProperty(VectorItem::P_X, 1.0);
  // EXPECT_EQ(item->data<std::string>(), "(1, 0, 0)");
}
