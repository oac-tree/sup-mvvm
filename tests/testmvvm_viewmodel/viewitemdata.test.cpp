// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#include "mvvm/viewmodel/viewitemdata.h"

#include <gtest/gtest.h>

//! Tests for Instruction class.

class ViewItemDataTest : public ::testing::Test
{
public:
};

//! Initial state.

TEST_F(ViewItemDataTest, InitialState)
{
  ModelView::ViewItemData item;
  EXPECT_FALSE(item.data().isValid());
  EXPECT_FALSE(item.data(Qt::EditRole).isValid());
  EXPECT_FALSE(item.data(Qt::DisplayRole).isValid());
  EXPECT_FALSE(item.data(Qt::ToolTipRole).isValid());

  EXPECT_TRUE(item.isEnabled());
  EXPECT_TRUE(item.isEditable());
}

TEST_F(ViewItemDataTest, setAppearance)
{
  ModelView::ViewItemData item;

  item.setEditable(false);
  EXPECT_TRUE(item.isEnabled());
  EXPECT_FALSE(item.isEditable());

  item.setEnabled(false);
  EXPECT_FALSE(item.isEnabled());
  EXPECT_FALSE(item.isEditable());
}

TEST_F(ViewItemDataTest, setData)
{
  ModelView::ViewItemData item;

  EXPECT_FALSE(item.data().isValid());

  EXPECT_TRUE(item.setData(QVariant::fromValue(42)));
  EXPECT_EQ(item.data().toInt(), 42);

  EXPECT_TRUE(item.setData(QVariant::fromValue(42)));
  EXPECT_EQ(item.data().toInt(), 42);

  EXPECT_TRUE(item.setData(QVariant::fromValue(1.2), Qt::EditRole));
  EXPECT_EQ(item.data().toDouble(), 1.2);

  EXPECT_TRUE(item.setData(QString("abc"), Qt::DisplayRole));
  EXPECT_EQ(item.data(Qt::DisplayRole).toString(), QString("abc"));

  EXPECT_TRUE(item.isEnabled());
  EXPECT_TRUE(item.isEditable());
}
