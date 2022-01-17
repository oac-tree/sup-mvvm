// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/standarditems/linkeditem.h"

#include "mvvm/model/itempool.h"
#include "mvvm/model/propertyitem.h"
#include "mvvm/model/sessionmodel.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! LinkedItem tests.

class LinkedItemTest : public ::testing::Test
{
};

//! Initial state of item when it is created outside of model context.

TEST_F(LinkedItemTest, initialState)
{
  LinkedItem item;
  EXPECT_EQ(item.Get(), nullptr);
  EXPECT_EQ(item.Data<std::string>(), std::string());
}

//! Link in single model context.

TEST_F(LinkedItemTest, sameModelContext)
{
  SessionModel model;
  auto item = model.InsertItem<PropertyItem>();
  auto link = model.InsertItem<LinkedItem>();

  // no link by default
  EXPECT_EQ(link->Get(), nullptr);

  // setting the link
  link->SetLink(item);

  // now linked
  EXPECT_EQ(link->Get(), item);
  EXPECT_EQ(link->Data<std::string>(), item->GetIdentifier());
}

//! Link in different model context.

TEST_F(LinkedItemTest, differentModelContext)
{
  auto pool = std::make_shared<ItemPool>();

  SessionModel model1("TestModel1", pool);
  SessionModel model2("TestModel2", pool);

  auto item = model1.InsertItem<PropertyItem>();
  auto link = model2.InsertItem<LinkedItem>();

  // no link by default
  EXPECT_EQ(link->Get(), nullptr);

  // setting the link
  link->SetLink(item);

  // now linked
  EXPECT_EQ(link->Get(), item);
}

//! FIXME enable test

// TEST_F(LinkedItemTest, onSetLink)
//{
//     SessionModel model;
//     auto item = model.insertItem<PropertyItem>();
//     auto link = model.insertItem<LinkedItem>();

//    // no link by default
//    EXPECT_EQ(link->get(), nullptr);

//    MockWidgetForItem widget(link);

//    auto expected_role = ItemDataRole::DATA;
//    auto expected_item = link;
//    EXPECT_CALL(widget, onDataChange(expected_item, expected_role)).Times(1);
//    EXPECT_CALL(widget, onPropertyChange(_, _)).Times(0);

//    // making action
//    link->setLink(item);
//}

//! Link in different model context.

TEST_F(LinkedItemTest, setNullAsLink)
{
  auto pool = std::make_shared<ItemPool>();

  SessionModel model("TestModel", pool);
  auto link = model.InsertItem<LinkedItem>();
  auto item = model.InsertItem<PropertyItem>();

  // no link by default
  EXPECT_EQ(link->Get(), nullptr);

  // setting link
  link->SetLink(item);
  EXPECT_EQ(link->Get(), item);

  // still null link
  link->SetLink(nullptr);
  EXPECT_EQ(link->Get(), nullptr);
  EXPECT_TRUE(utils::IsValid(link->Data()));
  EXPECT_EQ(link->Data<std::string>(), std::string());
}
