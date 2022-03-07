// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/project/projectchangecontroller.h"

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/propertyitem.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for ProjectChangeController class.

class ProjectChangeControllerTest : public ::testing::Test
{
};

TEST_F(ProjectChangeControllerTest, initialState)
{
  ApplicationModel sample_model("SampleModel");
  ApplicationModel material_model("MaterialModel");
  std::vector<ApplicationModel*> models = {&sample_model, &material_model};

  ProjectChangedController controller(models);
  EXPECT_FALSE(controller.hasChanged());
}

TEST_F(ProjectChangeControllerTest, twoModelsChange)
{
  ApplicationModel sample_model("SampleModel");
  ApplicationModel material_model("MaterialModel");
  std::vector<ApplicationModel*> models = {&sample_model, &material_model};

  ProjectChangedController controller(models);

  sample_model.InsertItem<PropertyItem>();
  material_model.InsertItem<PropertyItem>();

  EXPECT_TRUE(controller.hasChanged());

  controller.resetChanged();
  EXPECT_FALSE(controller.hasChanged());
}

TEST_F(ProjectChangeControllerTest, callback)
{
  int model_changed_count{0};

  ApplicationModel sample_model("SampleModel");
  ApplicationModel material_model("MaterialModel");
  std::vector<ApplicationModel*> models = {&sample_model, &material_model};

  auto on_model_changed = [&model_changed_count]() { ++model_changed_count; };
  ProjectChangedController controller(models, on_model_changed);

  // changing first model
  sample_model.InsertItem<PropertyItem>();
  EXPECT_TRUE(controller.hasChanged());
  EXPECT_EQ(model_changed_count, 1);

  // changing second model
  material_model.InsertItem<PropertyItem>();
  EXPECT_TRUE(controller.hasChanged());
  EXPECT_EQ(model_changed_count, 1);  // controller reports only once

  controller.resetChanged();
  EXPECT_FALSE(controller.hasChanged());
}
