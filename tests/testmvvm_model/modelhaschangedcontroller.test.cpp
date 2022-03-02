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

#include "mvvm/project/modelhaschangedcontroller.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/propertyitem.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for ModelHasChangedController class.

class ModelHasChangedControllerTest : public ::testing::Test
{
};

//! Tests initial state of the controller.

TEST_F(ModelHasChangedControllerTest, initialState)
{
  ApplicationModel model;
  ModelHasChangedController controller(&model);
  EXPECT_FALSE(controller.hasChanged());
}

//! Tests if controller 'sees' item insertion.

TEST_F(ModelHasChangedControllerTest, insertItem)
{
  ApplicationModel model;
  ModelHasChangedController controller(&model);

  model.InsertItem<PropertyItem>();
  EXPECT_TRUE(controller.hasChanged());

  controller.resetChanged();
  EXPECT_FALSE(controller.hasChanged());
}

//! Tests if controller sees item insertion.

TEST_F(ModelHasChangedControllerTest, removeItem)
{
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();

  ModelHasChangedController controller(&model);
  EXPECT_FALSE(controller.hasChanged());

  model.RemoveItem(item);

  EXPECT_TRUE(controller.hasChanged());
}

//! Tests if controller sees item data change.

TEST_F(ModelHasChangedControllerTest, dataChanged)
{
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();

  ModelHasChangedController controller(&model);
  EXPECT_FALSE(controller.hasChanged());

  item->SetData(42.0);
  EXPECT_TRUE(controller.hasChanged());
}

//! Tests if controller sees model reset.

TEST_F(ModelHasChangedControllerTest, modelReset)
{
  ApplicationModel model;
  model.InsertItem<PropertyItem>();

  ModelHasChangedController controller(&model);
  EXPECT_FALSE(controller.hasChanged());

  model.Clear();
  EXPECT_TRUE(controller.hasChanged());
}

//! Tests callback functioning.

TEST_F(ModelHasChangedControllerTest, callback)
{
  int change_count{0};
  auto on_change = [&change_count]() { change_count++; };

  ApplicationModel model;
  ModelHasChangedController controller(&model, on_change);

  model.InsertItem<PropertyItem>();
  EXPECT_TRUE(controller.hasChanged());
  EXPECT_EQ(change_count, 1);

  controller.resetChanged();
  EXPECT_FALSE(controller.hasChanged());
  EXPECT_EQ(change_count, 1);
}

//! Tests callback functioning.

TEST_F(ModelHasChangedControllerTest, timeOfLife)
{
  int change_count{0};
  auto on_change = [&change_count]() { change_count++; };

  ApplicationModel model;
  auto controller = std::make_unique<ModelHasChangedController>(&model, on_change);

  // change the model, check controller
  model.InsertItem<PropertyItem>();
  EXPECT_TRUE(controller->hasChanged());
  EXPECT_EQ(change_count, 1);

  // remove controller, change the model
  controller.reset();
  model.InsertItem<PropertyItem>();
  EXPECT_EQ(change_count, 1);
}
