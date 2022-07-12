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

#include "mock_model.h"
#include "mvvm/model/abstract_model_decorator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>

using namespace mvvm;
using ::testing::_;

class AbstractModelDecoratorTests : public ::testing::Test
{
public:
};

TEST_F(AbstractModelDecoratorTests, InitialState)
{
  auto decorated_model = std::make_unique<SessionModel>("TestModel");
  auto model_ptr = decorated_model.get();

  AbstractModelDecorator decorator(std::move(decorated_model));
  EXPECT_EQ(decorator.GetRootItem(), model_ptr->GetRootItem());
  EXPECT_EQ(decorator.GetType(), std::string("TestModel"));
}

TEST_F(AbstractModelDecoratorTests, InsertItem)
{
  MockModel mock_model;

  auto decorated_model = std::make_unique<MockModelDecorator>(&mock_model);

  AbstractModelDecorator decorator(std::move(decorated_model));

  EXPECT_CALL(mock_model, InsertItem(_, _, _)).Times(1);

  decorator.InsertItem(std::make_unique<SessionItem>(), decorator.GetRootItem(), {});
}
