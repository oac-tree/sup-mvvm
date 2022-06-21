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

#include "mvvm/project/project_change_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/property_item.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for ProjectChangeController class.

class ProjectChangeControllerTests : public ::testing::Test
{
};

TEST_F(ProjectChangeControllerTests, InitialState)
{
  ApplicationModel sample_model("SampleModel");
  ApplicationModel material_model("MaterialModel");
  std::vector<ApplicationModel*> models = {&sample_model, &material_model};

  ProjectChangedController controller(models);
  EXPECT_FALSE(controller.IsChanged());
}

TEST_F(ProjectChangeControllerTests, TwoModelsChange)
{
  ApplicationModel sample_model("SampleModel");
  ApplicationModel material_model("MaterialModel");
  std::vector<ApplicationModel*> models = {&sample_model, &material_model};

  ProjectChangedController controller(models);

  sample_model.InsertItem<PropertyItem>();
  material_model.InsertItem<PropertyItem>();

  EXPECT_TRUE(controller.IsChanged());

  controller.ResetIsChanged();
  EXPECT_FALSE(controller.IsChanged());
}

TEST_F(ProjectChangeControllerTests, Callback)
{
  int model_changed_count{0};

  ApplicationModel sample_model("SampleModel");
  ApplicationModel material_model("MaterialModel");
  std::vector<ApplicationModel*> models = {&sample_model, &material_model};

  auto on_model_changed = [&model_changed_count]() { ++model_changed_count; };
  ProjectChangedController controller(models, on_model_changed);

  // changing first model
  sample_model.InsertItem<PropertyItem>();
  EXPECT_TRUE(controller.IsChanged());
  EXPECT_EQ(model_changed_count, 1);

  // changing second model
  material_model.InsertItem<PropertyItem>();
  EXPECT_TRUE(controller.IsChanged());
  EXPECT_EQ(model_changed_count, 1);  // controller reports only once

  controller.ResetIsChanged();
  EXPECT_FALSE(controller.IsChanged());
}
