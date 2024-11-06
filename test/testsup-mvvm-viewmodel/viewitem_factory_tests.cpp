/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <mvvm/model/session_item.h>
#include <mvvm/model/session_model.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/viewmodel_utils.h>
#include <mvvm/viewmodelbase/viewitem.h>
#include <mvvm/viewmodelbase/viewmodel_base_utils.h>

#include <gtest/gtest.h>

#include <QColor>

namespace mvvm
{

/**
 * @brief Tests for helper methods from viewitem_factory.h
 */
class ViewItemFactoryTest : public ::testing::Test
{
public:
};

TEST_F(ViewItemFactoryTest, CreateDisplayNameViewItem)
{
  SessionItem item;
  item.SetDisplayName("abc");

  auto viewitem = CreateDisplayNameViewItem(&item);

  // presentation has a display role, and edit role
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
  EXPECT_EQ(viewitem->Data(Qt::EditRole).toString().toStdString(), item.GetDisplayName());

  EXPECT_FALSE(viewitem->SetData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(viewitem->SetData(QString("bbb"), Qt::EditRole));

  // data is the same as before, despite of all attempts to change it
  EXPECT_EQ(viewitem->Data(Qt::EditRole).toString().toStdString(), item.GetDisplayName());
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
}

//! Testing CreateDisplayNameViewItem() function when kEditableDisplayName is set.
TEST_F(ViewItemFactoryTest, CreateDisplayNameViewItemWhenEditableDisplayNameFlag)
{
  SessionItem item;
  item.SetDisplayName("abc");
  item.SetFlag(Appearance::kEditableDisplayName, true);

  auto viewitem = CreateEditableDisplayNameViewItem(&item);

  // presentation has a display role, which coincide with SessionItem::GetDisplayName, the rest is
  // blocked
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
  EXPECT_EQ(viewitem->Data(Qt::EditRole).toString().toStdString(), item.GetDisplayName());

  EXPECT_FALSE(viewitem->SetData(QString("aaa"), Qt::DisplayRole));
  EXPECT_TRUE(viewitem->SetData(QString("bbb"), Qt::EditRole));

  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
}

//! Checking that context method can't cast to underlying item.
TEST_F(ViewItemFactoryTest, CreateDisplayNameViewItemAndContext)
{
  VectorItem item;
  item.SetDisplayName("abc");

  auto viewitem = CreateDisplayNameViewItem(&item);

  // context can be casted to original item
  EXPECT_EQ(utils::GetItemFromView<SessionItem>(viewitem.get()), &item);
  EXPECT_EQ(utils::GetItemFromView<VectorItem>(viewitem.get()), &item);
}

//! Testing CreateDataViewItem (case of integer Data).
TEST_F(ViewItemFactoryTest, CreateDataViewItem)
{
  const int value{42};

  SessionItem item;
  item.SetData(value, DataRole::kData);

  auto viewitem = CreateDataViewItem(&item);

  // item has a display role, which coincide with data
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toInt(), value);
  EXPECT_EQ(viewitem->Data(Qt::EditRole).toInt(), value);

  const int new_value{43};
  // not sure if it's correct, but current convention is to disallow set another display role
  EXPECT_FALSE(viewitem->SetData(new_value, Qt::DisplayRole));
  EXPECT_TRUE(viewitem->SetData(new_value, Qt::EditRole));

  // data is the new one
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toInt(), new_value);
  EXPECT_EQ(viewitem->Data(Qt::EditRole).toInt(), new_value);
}

//! Testing CreateDataViewItem (case of integer Data).
TEST_F(ViewItemFactoryTest, CreateDataViewItemForSecondaryRole)
{
  const int kSecondaryDataRole{42};
  const int value{42};
  const std::string secondary_value{"abc"};

  SessionItem item;
  item.SetData(value, DataRole::kData);
  item.SetData(secondary_value, kSecondaryDataRole);

  auto viewitem = CreateDataViewItem(&item, kSecondaryDataRole);

  // item has a display role, which coincide with data
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString(), QString::fromStdString(secondary_value));
  EXPECT_EQ(viewitem->Data(Qt::EditRole).toString(), QString::fromStdString(secondary_value));

  const std::string new_secondary_value{"def"};
  // not sure if it's correct, but current convention is to disallow set another display role
  EXPECT_FALSE(viewitem->SetData(QString::fromStdString(new_secondary_value), Qt::DisplayRole));
  EXPECT_TRUE(viewitem->SetData(QString::fromStdString(new_secondary_value), Qt::EditRole));

  // data is the new one
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString(),
            QString::fromStdString(new_secondary_value));
  EXPECT_EQ(viewitem->Data(Qt::EditRole).toString(), QString::fromStdString(new_secondary_value));
}

//! Testing CreateDataViewItem (case of std::string Data).
TEST_F(ViewItemFactoryTest, CreateDataViewItemString)
{
  const std::string value{"abc"};

  SessionItem item;
  item.SetData(value, DataRole::kData);

  auto viewitem = CreateDataViewItem(&item);

  // item has a display role, which coincide with attribute value
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString().toStdString(), value);
  EXPECT_EQ(viewitem->Data(Qt::EditRole).toString().toStdString(), value);

  const QString new_value{"cde"};
  // not sure if it's correct, but current convention is to disallow set another display role
  EXPECT_FALSE(viewitem->SetData(new_value, Qt::DisplayRole));
  EXPECT_TRUE(viewitem->SetData(new_value, Qt::EditRole));

  // data is the new one
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString(), new_value);
  EXPECT_EQ(viewitem->Data(Qt::EditRole).toString(), new_value);
}

TEST_F(ViewItemFactoryTest, CreateEditableDisplayNameViewItem)
{
  SessionItem item;
  item.SetDisplayName("abc");

  auto viewitem = CreateEditableDisplayNameViewItem(&item);

  // presentation has a display role, which coincide with SessionItem::GetDisplayName, the rest is
  // blocked
  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
  EXPECT_EQ(viewitem->Data(Qt::EditRole).toString().toStdString(), item.GetDisplayName());

  EXPECT_FALSE(viewitem->SetData(QString("aaa"), Qt::DisplayRole));
  EXPECT_TRUE(viewitem->SetData(QString("bbb"), Qt::EditRole));

  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString().toStdString(), item.GetDisplayName());
}

TEST_F(ViewItemFactoryTest, CreateFixedDataViewItem)
{
  SessionItem item;
  item.SetDisplayName("abc");

  const QString expected_label("abc");
  auto viewitem = CreateFixedDataViewItem(&item, {{Qt::DisplayRole, QVariant(expected_label)}});
  // setting decoration role too
  const QColor expected_color(Qt::red);
  viewitem->SetData(QVariant::fromValue(expected_color), Qt::DecorationRole);

  EXPECT_EQ(viewitem->Data(Qt::DisplayRole).toString(), expected_label);
  EXPECT_FALSE(viewitem->Data(Qt::EditRole).isValid());
  EXPECT_EQ(viewitem->Data(Qt::DecorationRole).value<QColor>(), expected_color);

  Qt::ItemFlags expected_flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
  EXPECT_EQ(viewitem->Flags(), expected_flags);
}
}  // namespace mvvm
