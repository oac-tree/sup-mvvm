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

#include "mvvm/model/model_decorator.h"
#include "mvvm/model/sessionmodel.h"

#include <gtest/gtest.h>

#include <memory>

using namespace mvvm;

//! Testing templated class ModelDecorator.

class ModelDecoratorTests : public ::testing::Test
{
public:
  class MultiplyByTwoModel : public AbstractModelDecorator
  {
  public:
    MultiplyByTwoModel(std::unique_ptr<SessionModelInterface> decorated_model)
        : AbstractModelDecorator(std::move(decorated_model))
    {
    }
    bool SetData(SessionItem* item, const variant_t& value, int role) override
    {
      auto value_to_set = std::get<int>(value) * 2;  // just multiply given value by 2
      return m_decorated_model->SetData(item, value_to_set, role);
    }
  };

  class AddFourtyTwoModel : public AbstractModelDecorator
  {
  public:
    AddFourtyTwoModel(std::unique_ptr<SessionModelInterface> decorated_model)
        : AbstractModelDecorator(std::move(decorated_model))
    {
    }
    bool SetData(SessionItem* item, const variant_t& value, int role) override
    {
      auto value_to_set = std::get<int>(value) + 42;  // just add 42
      return m_decorated_model->SetData(item, value_to_set, role);
    }
  };
};

//! Creates nested model with parameter pack.

TEST_F(ModelDecoratorTests, CreateModel)
{
  auto model = CreateModel<AddFourtyTwoModel, SessionModel>("TestModel");
  auto item = model->InsertItem(std::make_unique<SessionItem>(), model->GetRootItem(), {});

  EXPECT_TRUE(model->SetData(item, 3, DataRole::kData));

  EXPECT_EQ(model->GetType(), std::string("TestModel"));
  EXPECT_EQ(item->Data<int>(), 45);  // 3 + 42 as provided by decorator
}

//! Creates two level decorator on top of SessionModel.

TEST_F(ModelDecoratorTests, TwoStepDecorator)
{
  ModelDecorator<MultiplyByTwoModel, AddFourtyTwoModel, SessionModel> decorator;

  auto item = decorator.InsertItem(std::make_unique<SessionItem>(), decorator.GetRootItem(), {});

  EXPECT_TRUE(decorator.SetData(item, 3, DataRole::kData));

  EXPECT_EQ(item->Data<int>(), 48);  // 3*2 + 42 as provided by decorators
}
