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

#include "mvvm/viewmodel/standard_row_strategies.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/viewmodel_utils.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <gtest/gtest.h>

#include <iostream>

namespace
{
const int expected_column_count = 2;
const QStringList expected_labels = QStringList() << "Name"
                                                  << "Value";
}  // namespace

using namespace mvvm;

class StandardRowStrategiesTest : public ::testing::Test
{
};

TEST_F(StandardRowStrategiesTest, LabelDataRowStrategyInitialState)
{
  LabelDataRowStrategy constructor;
  EXPECT_EQ(constructor.ConstructRow(nullptr).size(), 0);
  EXPECT_EQ(constructor.GetHorizontalHeaderLabels(), expected_labels);
}

//! Checks row construction for standard top level item, like Level, MultiLayer etc.

TEST_F(StandardRowStrategiesTest, LabelDataRowStrategyRowForTopLevelItem)
{
  SessionItem item;

  LabelDataRowStrategy constructor;
  auto items = constructor.ConstructRow(&item);
  EXPECT_EQ(items.size(), expected_column_count);  // label and empty items
  EXPECT_EQ(constructor.GetHorizontalHeaderLabels(), expected_labels);

  // non editable display name
  auto view_item = items.at(0).get();
  EXPECT_EQ(view_item->data(Qt::DisplayRole).toString().toStdString(), item.GetType());
  EXPECT_FALSE(view_item->data(Qt::EditRole).isValid());
  EXPECT_FALSE(view_item->setData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(view_item->setData(QString("bbb"), Qt::EditRole));
  EXPECT_EQ(utils::GetItemFromView<SessionItem>(view_item), &item);

  view_item = items.at(1).get();
  EXPECT_FALSE(view_item->data(Qt::DisplayRole).isValid());
  EXPECT_FALSE(view_item->data(Qt::EditRole).isValid());
}

//! Checks row construction for property item.

TEST_F(StandardRowStrategiesTest, LabelDataRowStrategyRowForPropertyItem)
{
  SessionItem item;
  std::string expected_name{"test_name"};
  item.SetData(expected_name);

  LabelDataRowStrategy constructor;
  auto items = constructor.ConstructRow(&item);
  EXPECT_EQ(items.size(), expected_column_count);
  EXPECT_EQ(constructor.GetHorizontalHeaderLabels(), expected_labels);

  // first cell with Type is readonly
  auto label_view_item = items.at(0).get();
  EXPECT_EQ(label_view_item->data(Qt::DisplayRole).toString().toStdString(), item.GetType());
  EXPECT_FALSE(label_view_item->data(Qt::EditRole).isValid());
  EXPECT_FALSE(label_view_item->setData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(label_view_item->setData(QString("bbb"), Qt::EditRole));

  // second cell has editable name
  auto data_view_item = items.at(1).get();
  EXPECT_EQ(data_view_item->data(Qt::DisplayRole).toString().toStdString(), expected_name);
  EXPECT_EQ(data_view_item->data(Qt::EditRole).toString().toStdString(), expected_name);
  // by current convention it is not possible to change display role
  EXPECT_FALSE(data_view_item->setData(QString("aaa"), Qt::DisplayRole));
  EXPECT_EQ(data_view_item->data(Qt::DisplayRole).toString().toStdString(), expected_name);
  EXPECT_EQ(data_view_item->data(Qt::EditRole).toString().toStdString(), expected_name);
  // but it is possible to set a new data, then editable data and display data changes
  EXPECT_TRUE(data_view_item->setData(QString("bbb"), Qt::EditRole));
  EXPECT_EQ(data_view_item->data(Qt::DisplayRole).toString().toStdString(), "bbb");
  EXPECT_EQ(data_view_item->data(Qt::EditRole).toString().toStdString(), "bbb");
}

TEST_F(StandardRowStrategiesTest, PropertiesRowStrategyInitialState)
{
  PropertiesRowStrategy strategy;
  EXPECT_EQ(strategy.ConstructRow(nullptr).size(), 0);
  EXPECT_EQ(strategy.GetHorizontalHeaderLabels(), QStringList());
}

//! Checks row construction for standard top level item. It shouldn't generate any rows.

TEST_F(StandardRowStrategiesTest, PropertiesRowStrategyTopLevelItem)
{
  SessionItem item;

  PropertiesRowStrategy strategy;
  auto items = strategy.ConstructRow(&item);
  EXPECT_EQ(items.size(), 0);
  EXPECT_EQ(strategy.GetHorizontalHeaderLabels(), QStringList());
}

//! Checks row construction for property item. It shouldn't generate any rows.

TEST_F(StandardRowStrategiesTest, PropertiesRowStrategyPropertyItem)
{
  SessionItem item;
  item.SetData(42.0);

  PropertiesRowStrategy strategy;
  auto items = strategy.ConstructRow(&item);
  EXPECT_EQ(items.size(), 0);
  EXPECT_EQ(strategy.GetHorizontalHeaderLabels(), QStringList());
}

//! Checks row construction for vector item.
//! There should be 3 view items looking to x, y, z properties.

TEST_F(StandardRowStrategiesTest, PropertiesRowStrategyVectorItemCustomLabels)
{
  VectorItem item;

  PropertiesRowStrategy strategy({"a", "b", "c"});
  auto items = strategy.ConstructRow(&item);

  EXPECT_EQ(items.size(), 3);
  EXPECT_EQ(strategy.GetHorizontalHeaderLabels(), QStringList({"a", "b", "c"}));

  // views should look at 3 property items
  auto view_x = items.at(0).get();
  EXPECT_EQ(utils::GetItemFromView(view_x), item.GetItem(VectorItem::kX));

  auto view_y = items.at(1).get();
  EXPECT_EQ(utils::GetItemFromView(view_y), item.GetItem(VectorItem::kY));

  auto view_z = items.at(2).get();
  EXPECT_EQ(utils::GetItemFromView(view_z), item.GetItem(VectorItem::kZ));
}

//! Checks row label construction for vector item.

TEST_F(StandardRowStrategiesTest, PropertiesRowStrategyVectorItemAutoLabels)
{
  VectorItem item;

  PropertiesRowStrategy strategy;
  auto items = strategy.ConstructRow(&item);

  // Horizontal labels should be constructed from default display names of X, Y and Z items
  EXPECT_EQ(strategy.GetHorizontalHeaderLabels(), QStringList({"X", "Y", "Z"}));
}

//! Row construction for rootItem with single item inserted. Shouldn't generate any row.

TEST_F(StandardRowStrategiesTest, PropertiesRowStrategyBaseItemInModelContext)
{
  ApplicationModel model("testModel");

  PropertiesRowStrategy strategy;
  auto items = strategy.ConstructRow(model.GetRootItem());
  EXPECT_EQ(items.size(), 0);

  model.InsertItem<SessionItem>();
  items = strategy.ConstructRow(model.GetRootItem());
  EXPECT_EQ(items.size(), 0);
}

//! Row construction for rootItem with single item inserted. Shouldn't generate any row.

TEST_F(StandardRowStrategiesTest, PropertiesRowStrategyPropertyItemTree)
{
  ApplicationModel model;
  auto parent = model.InsertItem<CompoundItem>();
  parent->AddProperty("thickness", 0.0);

  parent->RegisterTag(TagInfo::CreateUniversalTag("universal_tag"));
  model.InsertItem<SessionItem>(parent, "universal_tag");

  PropertiesRowStrategy strategy;
  auto items = strategy.ConstructRow(model.GetRootItem());

  // root item doesn't have properties
  EXPECT_EQ(items.size(), 0);

  // parent has one registered property.
  items = strategy.ConstructRow(parent);
  EXPECT_EQ(items.size(), 1);
}

//! Row construction for rootItem when vectorItem is present. Shouldn't generate any row.

TEST_F(StandardRowStrategiesTest, PropertiesRowStrategyVectorItemInModelContext)
{
  ApplicationModel model;
  model.InsertItem<VectorItem>();

  PropertiesRowStrategy strategy;
  auto items = strategy.ConstructRow(model.GetRootItem());
  EXPECT_EQ(items.size(), 0);
}

//! Checks row construction for vector item when 'y' is hidden.
//! There should be 2 view items looking to 'x' and 'z' properties.

TEST_F(StandardRowStrategiesTest, PropertiesRowStrategyVectorItemWhenChildHidden)
{
  VectorItem item;
  item.GetItem(VectorItem::kY)->SetVisible(false);

  PropertiesRowStrategy strategy({"a", "b", "c"});
  auto items = strategy.ConstructRow(&item);

  // views should look at 2 property items
  auto view_x = items.at(0).get();
  EXPECT_EQ(utils::GetItemFromView(view_x), item.GetItem(VectorItem::kX));

  auto view_y = items.at(1).get();
  EXPECT_EQ(utils::GetItemFromView(view_y), item.GetItem(VectorItem::kZ));
}
