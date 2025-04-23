/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/standarditems/vector_item.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/model/session_model.h>

#include <gtest/gtest.h>

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

//! Checking label update in ApplicationModel context.
TEST_F(VectorItemTests, LabelUpdate)
{
  ApplicationModel model;
  auto item = model.InsertItem<VectorItem>();

  EXPECT_NE(item->GetSlot(), nullptr);

  EXPECT_EQ(item->Data<std::string>(), "(0, 0, 0)");

  // Modification of one of the property should lead to label update
  item->SetProperty(VectorItem::kX, 1.0);

  // Updated thanks to VectorItem::Activate
  EXPECT_EQ(item->Data<std::string>(), "(1, 0, 0)");

  // Removing item from the model
  auto taken = model.TakeItem(model.GetRootItem(), mvvm::TagIndex::Default(0));
  EXPECT_EQ(taken.get(), item);

  // after item was taken from the model, it looses its listening capabilities
  EXPECT_EQ(item->Data<std::string>(), "(1, 0, 0)");
  item->SetProperty(VectorItem::kX, 2.0);
  EXPECT_EQ(item->Data<std::string>(), "(1, 0, 0)");  // still old label
}

TEST_F(VectorItemTests, Clone)
{
  VectorItem item;
  item.SetXYZ(1.0, 2.0, 3.0);

  {  // deep copy
    auto clone = utils::CopyItem(item);
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
    auto clone = item.Clone();
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

//! Testing clone when vector is the part of the model.
//! Model should be cleared in the clone.

TEST_F(VectorItemTests, CloneWhenVectorIsPartOfModel)
{
  SessionModel model;
  auto item = model.InsertItem<VectorItem>();

  {  // deep copy
    auto clone = item->Clone();
    EXPECT_EQ(clone->GetModel(), nullptr);
    EXPECT_EQ(clone->GetParent(), nullptr);

    EXPECT_EQ(clone->GetItem(VectorItem::kX)->GetModel(), nullptr);
    EXPECT_EQ(clone->GetItem(VectorItem::kY)->GetModel(), nullptr);
    EXPECT_EQ(clone->GetItem(VectorItem::kZ)->GetModel(), nullptr);

    EXPECT_EQ(clone->GetItem(VectorItem::kX)->GetParent(), clone.get());
    EXPECT_EQ(clone->GetItem(VectorItem::kY)->GetParent(), clone.get());
    EXPECT_EQ(clone->GetItem(VectorItem::kZ)->GetParent(), clone.get());
  }

  {  // clone
    auto clone = item->Clone();
    EXPECT_EQ(clone->GetModel(), nullptr);
    EXPECT_EQ(clone->GetParent(), nullptr);

    EXPECT_EQ(clone->GetItem(VectorItem::kX)->GetModel(), nullptr);
    EXPECT_EQ(clone->GetItem(VectorItem::kY)->GetModel(), nullptr);
    EXPECT_EQ(clone->GetItem(VectorItem::kZ)->GetModel(), nullptr);

    EXPECT_EQ(clone->GetItem(VectorItem::kX)->GetParent(), clone.get());
    EXPECT_EQ(clone->GetItem(VectorItem::kY)->GetParent(), clone.get());
    EXPECT_EQ(clone->GetItem(VectorItem::kZ)->GetParent(), clone.get());
  }
}
