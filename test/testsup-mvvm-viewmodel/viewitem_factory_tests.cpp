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

#include "mvvm/viewmodel/viewitem_factory.h"

#include <mvvm/model/sessionitem.h>
#include <mvvm/model/sessionmodel.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/viewmodel_utils.h>
#include <mvvm/viewmodelbase/viewitem.h>
#include <mvvm/viewmodelbase/viewmodel_base_utils.h>

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for factory methods related to ViewItem.

class ViewItemFactoryTests : public ::testing::Test
{
public:
};

TEST_F(ViewItemFactoryTests, CreateDisplayNameViewItem)
{
  SessionItem item;
  item.SetDisplayName("abc");

  auto viewitem = CreateDisplayNameViewItem(&item);

  // presentation has a display role, which coincide with SessionItem::GetDisplayName, the rest is
  // blocked
  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
  EXPECT_FALSE(viewitem->data(Qt::EditRole).isValid());
  EXPECT_FALSE(viewitem->setData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(viewitem->setData(QString("bbb"), Qt::EditRole));

  // data is the same as before, despite of all attempts to change it
  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
}

//! Checking that context method can't cast to underlying item.

TEST_F(ViewItemFactoryTests, CreateDisplayNameViewItemAndContext)
{
  VectorItem item;
  item.SetDisplayName("abc");

  auto viewitem = CreateDisplayNameViewItem(&item);

  // Context can be casted to SessionItem
  EXPECT_EQ(utils::GetItemFromView<SessionItem>(viewitem.get()), &item);

  // To retrieve original item use GetItem method
  EXPECT_EQ(utils::GetItemFromView<VectorItem>(viewitem.get()), &item);
}

//! Testing CreateDataViewItem (case of integer data).

TEST_F(ViewItemFactoryTests, CreateDataViewItem)
{
  const int value{42};

  SessionItem item;
  item.SetData(value, DataRole::kData);

  auto viewitem = CreateDataViewItem(&item);

  // item has a display role, which coincide with data
  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toInt(), value);
  EXPECT_EQ(viewitem->data(Qt::EditRole).toInt(), value);

  const int new_value{43};
  // not sure if it's correct, but current convention is to disallow set another display role
  EXPECT_FALSE(viewitem->setData(new_value, Qt::DisplayRole));
  EXPECT_TRUE(viewitem->setData(new_value, Qt::EditRole));

  // data is the new one
  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toInt(), new_value);
  EXPECT_EQ(viewitem->data(Qt::EditRole).toInt(), new_value);
}

//! Testing CreateDataViewItem (case of integer data).

TEST_F(ViewItemFactoryTests, CreateDataViewItemForSecondaryRole)
{
  const int kSecondaryDataRole{42};
  const int value{42};
  const std::string secondary_value{"abc"};

  SessionItem item;
  item.SetData(value, DataRole::kData);
  item.SetData(secondary_value, kSecondaryDataRole);

  auto viewitem = CreateDataViewItem(&item, kSecondaryDataRole);

  // item has a display role, which coincide with data
  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toString(), QString::fromStdString(secondary_value));
  EXPECT_EQ(viewitem->data(Qt::EditRole).toString(), QString::fromStdString(secondary_value));

  const std::string new_secondary_value{"def"};
  // not sure if it's correct, but current convention is to disallow set another display role
  EXPECT_FALSE(viewitem->setData(QString::fromStdString(new_secondary_value), Qt::DisplayRole));
  EXPECT_TRUE(viewitem->setData(QString::fromStdString(new_secondary_value), Qt::EditRole));

  // data is the new one
  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toString(), QString::fromStdString(new_secondary_value));
  EXPECT_EQ(viewitem->data(Qt::EditRole).toString(), QString::fromStdString(new_secondary_value));
}

//! Testing CreateDataViewItem (case of std::string data).

TEST_F(ViewItemFactoryTests, CreateDataViewItemString)
{
  const std::string value{"abc"};

  SessionItem item;
  item.SetData(value, DataRole::kData);

  auto viewitem = CreateDataViewItem(&item);

  // item has a display role, which coincide with attribute value
  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toString().toStdString(), value);
  EXPECT_EQ(viewitem->data(Qt::EditRole).toString().toStdString(), value);

  const QString new_value{"cde"};
  // not sure if it's correct, but current convention is to disallow set another display role
  EXPECT_FALSE(viewitem->setData(new_value, Qt::DisplayRole));
  EXPECT_TRUE(viewitem->setData(new_value, Qt::EditRole));

  // data is the new one
  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toString(), new_value);
  EXPECT_EQ(viewitem->data(Qt::EditRole).toString(), new_value);
}

TEST_F(ViewItemFactoryTests, CreateEditableDisplayNameViewItem)
{
  SessionItem item;
  item.SetDisplayName("abc");

  auto viewitem = CreateEditableDisplayNameViewItem(&item);

  // presentation has a display role, which coincide with SessionItem::GetDisplayName, the rest is
  // blocked
  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
  EXPECT_EQ(viewitem->data(Qt::EditRole).toString().toStdString(), item.GetDisplayName());

  EXPECT_FALSE(viewitem->setData(QString("aaa"), Qt::DisplayRole));
  EXPECT_TRUE(viewitem->setData(QString("bbb"), Qt::EditRole));

  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
}
