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

#include "mvvm/experimental/reporting_model.h"
#include "mvvm/experimental/reporting_model_v2.h"
#include "mvvm/experimental/undoable_model.h"
#include "mvvm/experimental/abstract_model_decorator.h"
#include "mvvm/experimental/model.h"
#include "mvvm/experimental/item.h"

#include <gtest/gtest.h>

using namespace mvvm::experimental;

//! Testing Item.

class ReportingModelTests : public ::testing::Test
{
public:
  std::pair<std::unique_ptr<Item>, Item*> CreateTestData()
  {
    auto item = std::make_unique<Item>();
    auto item_ptr = item.get();
    return {std::move(item), item_ptr};
  }

};

TEST_F(ReportingModelTests, InitialState)
{
  ReportingModel<Model> model;
  EXPECT_NE(model.GetRootItem(), nullptr);
}

TEST_F(ReportingModelTests, SetData)
{
  ReportingModel<Model> model;
  auto [child, child_ptr] = CreateTestData();
  auto inserted = model.InsertItem(std::move(child), model.GetRootItem(), 0);

  EXPECT_TRUE(model.SetData(inserted, 42));
  EXPECT_FALSE(model.SetData(inserted, 42));
  EXPECT_EQ(inserted->GetData(), variant_t(42));

  UndoableModel<ReportingModel<Model>> model2;
  ReportingModel<UndoableModel<Model>> model3;
}

TEST_F(ReportingModelTests, ReportingModelV2)
{
  ReportingModelV2 model(std::make_unique<Model>());
}
