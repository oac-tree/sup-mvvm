/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/standarditems/vector_item.h"

#include <gtest/gtest.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/sessionmodel.h>

using namespace mvvm;

//! VectorItem tests.

class VectorItemTests : public ::testing::Test
{
};

//! Initial state of item when it is created outside of model context.

TEST_F(VectorItemTests, InitialState)
{
  const VectorItem item;

  EXPECT_FALSE(item.IsEditable());

  EXPECT_EQ(item.Property<double>(VectorItem::kX), 0.0);
  EXPECT_EQ(item.Property<double>(VectorItem::kY), 0.0);
  EXPECT_EQ(item.Property<double>(VectorItem::kZ), 0.0);

  // default label
  EXPECT_EQ(item.Data<std::string>(), "(0, 0, 0)");
}

//! Initial state of item in model context

TEST_F(VectorItemTests, InitialStateFromModel)
{
  SessionModel model;
  auto item = model.InsertItem<VectorItem>();

  EXPECT_EQ(item->Property<double>(VectorItem::kX), 0.0);
  EXPECT_EQ(item->Property<double>(VectorItem::kY), 0.0);
  EXPECT_EQ(item->Property<double>(VectorItem::kZ), 0.0);

  // default label
  EXPECT_EQ(item->Data<std::string>(), "(0, 0, 0)");
}

TEST_F(VectorItemTests, SetXYZ)
{
  SessionModel model;
  auto item = model.InsertItem<VectorItem>();
  item->SetXYZ(1.0, 2.0, 3.0);

  EXPECT_EQ(item->X(), 1.0);
  EXPECT_EQ(item->Y(), 2.0);
  EXPECT_EQ(item->Z(), 3.0);
}

//! Checking label update in ApplicationModel context
//! FIXME uncomment test

// TEST_F(VectorItemTests, LabelUpdate)
//{
//   ApplicationModel model;
//   auto item = model.InsertItem<VectorItem>();

//  EXPECT_EQ(item->Data<std::string>(), "(0, 0, 0)");

//  // Modification of one of the property should lead to label update
//  item->SetProperty(VectorItem::kX, 1.0);

//  // Updated thanks to VectorItem::Activate
//  EXPECT_EQ(item->Data<std::string>(), "(1, 0, 0)");
//}

TEST_F(VectorItemTests, Clone)
{
  VectorItem item;
  item.SetXYZ(1.0, 2.0, 3.0);

  {  // deep copy
    auto clone = item.Clone(/* make_unique_id*/ true);
    auto vector_clone = dynamic_cast<VectorItem*>(clone.get());

    ASSERT_NE(vector_clone, nullptr);

    EXPECT_NE(vector_clone->GetIdentifier(), item.GetIdentifier());
    EXPECT_DOUBLE_EQ(vector_clone->X(), 1.0);
    EXPECT_DOUBLE_EQ(vector_clone->Y(), 2.0);
    EXPECT_DOUBLE_EQ(vector_clone->Z(), 3.0);

    auto cloned_x = dynamic_cast<PropertyItem*>(vector_clone->GetItem(VectorItem::kX));
    auto cloned_y = dynamic_cast<PropertyItem*>(vector_clone->GetItem(VectorItem::kY));
    auto cloned_z = dynamic_cast<PropertyItem*>(vector_clone->GetItem(VectorItem::kZ));
    ASSERT_NE(cloned_x, nullptr);
    ASSERT_NE(cloned_y, nullptr);
    ASSERT_NE(cloned_z, nullptr);

    EXPECT_NE(cloned_x, item.GetItem(VectorItem::kX));
    EXPECT_NE(cloned_y, item.GetItem(VectorItem::kY));
    EXPECT_NE(cloned_z, item.GetItem(VectorItem::kZ));

    EXPECT_NE(cloned_x->GetIdentifier(), item.GetItem(VectorItem::kX)->GetIdentifier());
    EXPECT_NE(cloned_y->GetIdentifier(), item.GetItem(VectorItem::kY)->GetIdentifier());
    EXPECT_NE(cloned_z->GetIdentifier(), item.GetItem(VectorItem::kZ)->GetIdentifier());
  }

  {  // clone
    auto clone = item.Clone(/* make_unique_id*/ false);
    auto vector_clone = dynamic_cast<VectorItem*>(clone.get());

    ASSERT_NE(vector_clone, nullptr);

    EXPECT_EQ(vector_clone->GetIdentifier(), item.GetIdentifier());
    EXPECT_DOUBLE_EQ(vector_clone->X(), 1.0);
    EXPECT_DOUBLE_EQ(vector_clone->Y(), 2.0);
    EXPECT_DOUBLE_EQ(vector_clone->Z(), 3.0);

    auto x_clone = dynamic_cast<PropertyItem*>(vector_clone->GetItem(VectorItem::kX));
    auto y_clone = dynamic_cast<PropertyItem*>(vector_clone->GetItem(VectorItem::kY));
    auto z_clone = dynamic_cast<PropertyItem*>(vector_clone->GetItem(VectorItem::kZ));
    ASSERT_NE(x_clone, nullptr);
    ASSERT_NE(y_clone, nullptr);
    ASSERT_NE(z_clone, nullptr);

    EXPECT_NE(x_clone, item.GetItem(VectorItem::kX));
    EXPECT_NE(y_clone, item.GetItem(VectorItem::kY));
    EXPECT_NE(z_clone, item.GetItem(VectorItem::kZ));

    EXPECT_EQ(x_clone->GetIdentifier(), item.GetItem(VectorItem::kX)->GetIdentifier());
    EXPECT_EQ(y_clone->GetIdentifier(), item.GetItem(VectorItem::kY)->GetIdentifier());
    EXPECT_EQ(z_clone->GetIdentifier(), item.GetItem(VectorItem::kZ)->GetIdentifier());
  }
}
