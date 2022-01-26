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

#include "mvvm/signals/itemlistener.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/standarditems/standarditemincludes.h"

#include <gtest/gtest.h>

using namespace mvvm;

class ItemListenerTest : public ::testing::Test
{
};

TEST_F(ItemListenerTest, InitialState)
{
  ItemListener<SessionItem> listener;
  EXPECT_EQ(listener.GetItem(), nullptr);
}

TEST_F(ItemListenerTest, SetItem)
{
  ItemListener<SessionItem> listener;

  // listener can't listen an item without the model
  SessionItem item1;
  EXPECT_THROW(listener.SetItem(&item1), std::runtime_error);

  // listener can't listen an item belonging to the model without notifications
  SessionModel session_model;
  auto item2 = session_model.InsertItem<PropertyItem>();
  EXPECT_THROW(listener.SetItem(item2), std::runtime_error);

  // set proper item
  ApplicationModel app_model;
  auto item3 = app_model.InsertItem<PropertyItem>();
  EXPECT_NO_THROW(listener.SetItem(item3));
  EXPECT_EQ(listener.GetItem(), item3);
}
