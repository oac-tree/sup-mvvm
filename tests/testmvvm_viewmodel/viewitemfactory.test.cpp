/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/viewitemfactory.h"

#include "mockmodelnotifier.h"

#include "mvvm/model/modelcomposer.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/sessionmodel.h"
#include "mvvm/standarditems/vectoritem.h"
#include "mvvm/viewmodelbase/viewitem.h"
#include "mvvm/viewmodelbase/viewmodelbaseutils.h"
#include "mvvm/viewmodel/viewmodelutils.h"

#include <gtest/gtest.h>

using namespace mvvm;

//! Tests for factory methods related to ViewItem.

class ViewItemFactoryTest : public ::testing::Test
{
public:
};

TEST_F(ViewItemFactoryTest, CreateDisplayNameViewItem)
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

TEST_F(ViewItemFactoryTest, CreateDisplayNameViewItemAndContext)
{
  VectorItem item;
  item.SetDisplayName("abc");

  auto viewitem = CreateDisplayNameViewItem(&item);

  // Context can be casted to SessionItem
  EXPECT_EQ(utils::GetContext<SessionItem>(viewitem.get()), &item);

  // But not to original VectorItem (since CreateDisplayNameViewItem() works with
  // PresentationItem<SessionItem>))
  EXPECT_NE(utils::GetContext<VectorItem>(viewitem.get()), &item);

  // To retrieve original item use GetItem method
  EXPECT_EQ(utils::GetItem<VectorItem>(viewitem.get()), &item);
}

//! Testing CreateDataViewItem (case of integer data).

TEST_F(ViewItemFactoryTest, CreateDataViewItem)
{
  const int value{42};

  SessionItem item;
  item.SetData(value, DataRole::kData);

  auto viewitem = CreateDataViewItem(&item, {});

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

//! Testing CreateDataViewItem (case of double data, set through the composer).

TEST_F(ViewItemFactoryTest, CreateDataViewItemViaComposer)
{
  const double value{42.1};
  SessionModel model;
  auto* item = model.InsertItem<SessionItem>();
  item->SetData(value, DataRole::kData);

  MockModelNotifier notifier;
  ModelComposer composer(&model, &notifier);
  auto set_data = [&composer](auto item, auto data, auto role)
  { return composer.SetData(item, data, role); };

  auto viewitem = CreateDataViewItem(item, set_data);

  EXPECT_CALL(notifier, DataChangedNotify(item, DataRole::kData)).Times(1);

  // the rest of the test is the same

  // item has a display role, which coincide with data
  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toDouble(), value);
  EXPECT_EQ(viewitem->data(Qt::EditRole).toDouble(), value);

  const double new_value{43.1};
  // not sure if it's correct, but current convention is to disallow set another display role
  EXPECT_FALSE(viewitem->setData(new_value, Qt::DisplayRole));
  EXPECT_TRUE(viewitem->setData(new_value, Qt::EditRole));

  // data is the new one
  EXPECT_EQ(viewitem->data(Qt::DisplayRole).toDouble(), new_value);
  EXPECT_EQ(viewitem->data(Qt::EditRole).toDouble(), new_value);
}

//! Testing CreateDataViewItem (case of std::string data).

TEST_F(ViewItemFactoryTest, CreateDataViewItemString)
{
  const std::string value{"abc"};

  SessionItem item;
  item.SetData(value, DataRole::kData);

  auto viewitem = CreateDataViewItem(&item, {});

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
