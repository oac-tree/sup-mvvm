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
  EXPECT_EQ(decorator.GetFactory(), model_ptr->GetFactory());
}

TEST_F(AbstractModelDecoratorTests, InsertItem)
{
  SessionItem expected_parent;
  const TagIndex expected_tagindex{"tag", 42};

  MockModel mock_model;
  auto decorated_model = std::make_unique<MockModelDecorator>(&mock_model);
  AbstractModelDecorator decorator(std::move(decorated_model));

  EXPECT_CALL(mock_model, GetType()).Times(0);
  EXPECT_CALL(mock_model, GetRootItem()).Times(0);
  EXPECT_CALL(mock_model, InsertItem(_, &expected_parent, expected_tagindex)).Times(1);
  EXPECT_CALL(mock_model, InsertNewItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, TakeItem(_, _)).Times(0);
  EXPECT_CALL(mock_model, RemoveItem(_)).Times(0);
  EXPECT_CALL(mock_model, MoveItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, SetData(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, Clear(_)).Times(0);

  decorator.InsertItem(std::make_unique<SessionItem>(), &expected_parent, expected_tagindex);
}

TEST_F(AbstractModelDecoratorTests, InsertNewItem)
{
  const std::string expected_type("expected_type");
  SessionItem expected_parent;
  const TagIndex expected_tagindex{"tag", 42};

  MockModel mock_model;
  auto decorated_model = std::make_unique<MockModelDecorator>(&mock_model);
  AbstractModelDecorator decorator(std::move(decorated_model));

  EXPECT_CALL(mock_model, GetType()).Times(0);
  EXPECT_CALL(mock_model, GetRootItem()).Times(0);
  EXPECT_CALL(mock_model, InsertItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, InsertNewItem(_, &expected_parent, expected_tagindex)).Times(1);
  EXPECT_CALL(mock_model, TakeItem(_, _)).Times(0);
  EXPECT_CALL(mock_model, RemoveItem(_)).Times(0);
  EXPECT_CALL(mock_model, MoveItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, SetData(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, Clear(_)).Times(0);

  decorator.InsertNewItem(expected_type, &expected_parent, expected_tagindex);
}

TEST_F(AbstractModelDecoratorTests, TakeItem)
{
  const std::string expected_type("expected_type");
  SessionItem expected_parent;
  const TagIndex expected_tagindex{"tag", 42};

  MockModel mock_model;
  auto decorated_model = std::make_unique<MockModelDecorator>(&mock_model);
  AbstractModelDecorator decorator(std::move(decorated_model));

  EXPECT_CALL(mock_model, GetType()).Times(0);
  EXPECT_CALL(mock_model, GetRootItem()).Times(0);
  EXPECT_CALL(mock_model, InsertItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, InsertNewItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, TakeItem(&expected_parent, expected_tagindex)).Times(1);
  EXPECT_CALL(mock_model, RemoveItem(_)).Times(0);
  EXPECT_CALL(mock_model, MoveItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, SetData(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, Clear(_)).Times(0);

  decorator.TakeItem(&expected_parent, expected_tagindex);
}

TEST_F(AbstractModelDecoratorTests, RemoveItem)
{
  SessionItem expected_item;

  MockModel mock_model;
  auto decorated_model = std::make_unique<MockModelDecorator>(&mock_model);
  AbstractModelDecorator decorator(std::move(decorated_model));

  EXPECT_CALL(mock_model, GetType()).Times(0);
  EXPECT_CALL(mock_model, GetRootItem()).Times(0);
  EXPECT_CALL(mock_model, InsertItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, InsertNewItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, TakeItem(_, _)).Times(0);
  EXPECT_CALL(mock_model, RemoveItem(&expected_item)).Times(1);
  EXPECT_CALL(mock_model, MoveItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, SetData(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, Clear(_)).Times(0);

  decorator.RemoveItem(&expected_item);
}

TEST_F(AbstractModelDecoratorTests, MoveItem)
{
  SessionItem expected_item;
  SessionItem expected_parent;
  const TagIndex expected_tagindex{"tag", 42};

  MockModel mock_model;
  auto decorated_model = std::make_unique<MockModelDecorator>(&mock_model);
  AbstractModelDecorator decorator(std::move(decorated_model));

  EXPECT_CALL(mock_model, GetType()).Times(0);
  EXPECT_CALL(mock_model, GetRootItem()).Times(0);
  EXPECT_CALL(mock_model, InsertItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, InsertNewItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, TakeItem(_, _)).Times(0);
  EXPECT_CALL(mock_model, RemoveItem(_)).Times(0);
  EXPECT_CALL(mock_model, MoveItem(&expected_item, &expected_parent, expected_tagindex)).Times(1);
  EXPECT_CALL(mock_model, SetData(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, Clear(_)).Times(0);

  decorator.MoveItem(&expected_item, &expected_parent, expected_tagindex);
}

TEST_F(AbstractModelDecoratorTests, SetData)
{
  SessionItem expected_item;
  const int expected_role{42};
  const variant_t expected_data(std::string("data"));

  MockModel mock_model;
  auto decorated_model = std::make_unique<MockModelDecorator>(&mock_model);
  AbstractModelDecorator decorator(std::move(decorated_model));

  EXPECT_CALL(mock_model, GetType()).Times(0);
  EXPECT_CALL(mock_model, GetRootItem()).Times(0);
  EXPECT_CALL(mock_model, InsertItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, InsertNewItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, TakeItem(_, _)).Times(0);
  EXPECT_CALL(mock_model, RemoveItem(_)).Times(0);
  EXPECT_CALL(mock_model, MoveItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, SetData(&expected_item, expected_data, expected_role)).Times(1);
  EXPECT_CALL(mock_model, Clear(_)).Times(0);

  decorator.SetData(&expected_item, expected_data, expected_role);
}

TEST_F(AbstractModelDecoratorTests, Clear)
{
  MockModel mock_model;
  auto decorated_model = std::make_unique<MockModelDecorator>(&mock_model);
  AbstractModelDecorator decorator(std::move(decorated_model));

  EXPECT_CALL(mock_model, GetType()).Times(0);
  EXPECT_CALL(mock_model, GetRootItem()).Times(0);
  EXPECT_CALL(mock_model, InsertItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, InsertNewItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, TakeItem(_, _)).Times(0);
  EXPECT_CALL(mock_model, RemoveItem(_)).Times(0);
  EXPECT_CALL(mock_model, MoveItem(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, SetData(_, _, _)).Times(0);
  EXPECT_CALL(mock_model, Clear(_)).Times(1);

  decorator.Clear();
}
