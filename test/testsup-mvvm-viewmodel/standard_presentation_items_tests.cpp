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

#include "mvvm/viewmodel/standard_presentation_items.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/standarditems/editor_constants.h>

#include <gtest/gtest.h>

#include <QColor>

using namespace mvvm;

class StandardPresentationItemsTest : public ::testing::Test
{
public:
  class TestPresentation : public SessionItemPresentation
  {
  public:
    TestPresentation(SessionItem* item, int role) : SessionItemPresentation(item, role) {}
  };
};

TEST_F(StandardPresentationItemsTest, SessionItemPresentationInitialState)
{
  SessionItem item;

  TestPresentation presentation(&item, 42);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetDataRole(), 42);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_FALSE(presentation.SetData(42, Qt::EditRole));
  EXPECT_TRUE(presentation.IsValidItemDataRole(42));
  EXPECT_FALSE(presentation.IsValidItemDataRole(43));
}

TEST_F(StandardPresentationItemsTest, DataPresentationItemDataForNoData)
{
  SessionItem item;

  EXPECT_THROW(DataPresentationItem(nullptr), RuntimeException);

  DataPresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetDataRole(), DataRole::kData);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());

  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::DisplayRole).isValid());

  EXPECT_TRUE(presentation.IsValidItemDataRole(DataRole::kAppearance));
  EXPECT_TRUE(presentation.IsValidItemDataRole(DataRole::kTooltip));
  EXPECT_TRUE(presentation.IsValidItemDataRole(DataRole::kData));
  EXPECT_FALSE(presentation.IsValidItemDataRole(DataRole::kDisplay));

  {
    auto roles = presentation.GetQtRoles(DataRole::kDisplay);
    EXPECT_TRUE(roles.isEmpty());
  }

  {
    auto roles = presentation.GetQtRoles(DataRole::kData);
    EXPECT_EQ(roles, QVector<int>({Qt::DisplayRole, Qt::EditRole}));
  }

  {
    auto roles = presentation.GetQtRoles(DataRole::kAppearance);
    EXPECT_EQ(roles, QVector<int>({Qt::ForegroundRole}));
  }

  {
    auto roles = presentation.GetQtRoles(DataRole::kTooltip);
    EXPECT_EQ(roles, QVector<int>({Qt::ToolTipRole}));
  }
}

TEST_F(StandardPresentationItemsTest, DisplayNamePresentationItem)
{
  SessionItem item;
  item.SetDisplayName("abc");

  DisplayNamePresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetDataRole(), DataRole::kDisplay);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());

  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("abc"));

  // for the moment it is not possible to set display name from a view
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::EditRole));
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::DisplayRole));
}

//! Testing tooltip tole.

TEST_F(StandardPresentationItemsTest, DisplayNamePresentationItemTooltipRole)
{
  SessionItem item;

  EXPECT_THROW(DisplayNamePresentationItem(nullptr), RuntimeException);

  DisplayNamePresentationItem presentation(&item);
  EXPECT_FALSE(presentation.Data(Qt::ToolTipRole).isValid());

  item.SetToolTip("abc");
  EXPECT_EQ(presentation.Data(Qt::ToolTipRole).toString(), QString("abc"));
}

//! Testing color of item depending on enable/disable status.

TEST_F(StandardPresentationItemsTest, DisplayNamePresentationItemForegroundRole)
{
  SessionItem item;
  item.SetData(42);

  DisplayNamePresentationItem presentation(&item);

  // Enabled item doesn't have valid foreground role.
  // This means that the color of text is left for a view to decide.
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  item.SetEnabled(false);
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole), QColor(Qt::gray));

  item.SetEnabled(true);
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  // Hidden item has the same color by default. It is up to the view to decide how to show "hidden"
  // item.

  item.SetVisible(false);
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole), QColor(Qt::gray));
}

TEST_F(StandardPresentationItemsTest, DisplayNamePresentationItemCheckStateRole)
{
  SessionItem item;
  item.SetData(false);
  DisplayNamePresentationItem presentation(&item);

  // no CheckStateRole for a display name
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());
}

TEST_F(StandardPresentationItemsTest, DataPresentationItemDataForDouble)
{
  const double expected{42.0};

  SessionItem item;
  item.SetData(expected);

  DataPresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetDataRole(), DataRole::kData);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());

  EXPECT_EQ(presentation.Data(Qt::EditRole), expected);
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), expected);
}

TEST_F(StandardPresentationItemsTest, DataPresentationItemSetDataForDouble)
{
  SessionItem item;
  item.SetData(42.0);

  DataPresentationItem presentation(&item);

  const double new_data{43.0};

  // not possible to set DisplayRole
  EXPECT_FALSE(presentation.SetData(QVariant::fromValue(new_data), Qt::DisplayRole));

  EXPECT_TRUE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QVariant::fromValue(new_data));  // new data
  EXPECT_EQ(presentation.Data(Qt::EditRole), QVariant::fromValue(new_data));     // new data

  // SessionItem itself should have new data
  EXPECT_EQ(item.Data<double>(), new_data);  // new data

  // attempt to set same data twice
  EXPECT_FALSE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));

  // it is not allowed to set another type of data
  QVariant not_allowed_value("Layer");
  EXPECT_THROW(presentation.SetData(not_allowed_value, Qt::EditRole), RuntimeException);
}

TEST_F(StandardPresentationItemsTest, DataPresentationItemForSecondaryDataRole)
{
  const int kSecondaryDataRole{42};
  SessionItem item;
  item.SetData(42.0);
  item.SetData(std::string("abc"), kSecondaryDataRole);

  DataPresentationItem presentation(&item, kSecondaryDataRole);

  const QString new_data("def");

  // not possible to set DisplayRole
  EXPECT_FALSE(presentation.SetData(QVariant::fromValue(new_data), Qt::DisplayRole));

  EXPECT_TRUE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QVariant::fromValue(new_data));  // new data
  EXPECT_EQ(presentation.Data(Qt::EditRole), QVariant::fromValue(new_data));     // new data

  // SessionItem itself should have new data
  EXPECT_EQ(item.Data<std::string>(kSecondaryDataRole), new_data.toStdString());  // new data

  // attempt to set same data twice
  EXPECT_FALSE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));

  //  // it is not allowed to set another type of data
  //  QVariant not_allowed_value("Layer");
  //  EXPECT_THROW(presentation.SetData(not_allowed_value, Qt::EditRole), RuntimeException);
}

//! Testing tooltip tole.

TEST_F(StandardPresentationItemsTest, DataPresentationItemTooltipRole)
{
  SessionItem item;

  DataPresentationItem presentation(&item);
  EXPECT_FALSE(presentation.Data(Qt::ToolTipRole).isValid());

  item.SetToolTip("abc");
  EXPECT_EQ(presentation.Data(Qt::ToolTipRole).toString(), QString("abc"));
}

//! Testing color of item depending on enable/disable status.

TEST_F(StandardPresentationItemsTest, DataPresentationItemForegroundRole)
{
  SessionItem item;
  item.SetData(42);

  DataPresentationItem presentation(&item);

  // Enabled item doesn't have valid foreground role.
  // This means that the color of text is left for a view to decide.
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  item.SetEnabled(false);
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole), QColor(Qt::gray));

  item.SetEnabled(true);
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  // Hidden item has the same color by default. It is up to the view to decide how to show "hidden"
  // item.

  item.SetVisible(false);
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole), QColor(Qt::gray));
}

//! Testing color of item depending on enable/disable status.

TEST_F(StandardPresentationItemsTest, DataPresentationItemCheckStateRole)
{
  // Item carrying integer doesn't have CheckStateRole
  SessionItem item1;
  item1.SetData(42);
  DataPresentationItem presentation1(&item1);
  EXPECT_FALSE(presentation1.Data(Qt::CheckStateRole).isValid());

  // Item carrying bool has a CheckStateRole
  SessionItem item2;
  item2.SetData(false);
  DataPresentationItem presentation2(&item2);
  EXPECT_TRUE(presentation2.Data(Qt::CheckStateRole).isValid());
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);

  item2.SetData(true);
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
}

//! Decoration role (string with additional property ColorEditor set)

TEST_F(StandardPresentationItemsTest, DataPresentationItemDecorationRole)
{
  // create SessionItem with data on board
  SessionItem item;
  EXPECT_TRUE(item.SetData("red"));
  item.SetEditorType(constants::kColorEditorType);

  DataPresentationItem viewItem(&item);
  EXPECT_EQ(viewItem.Data(Qt::EditRole), QString("red"));
  EXPECT_EQ(viewItem.Data(Qt::DisplayRole), QString("red"));
  EXPECT_EQ(viewItem.Data(Qt::DecorationRole).value<QColor>(), QColor("red"));
}

TEST_F(StandardPresentationItemsTest, CreateLabelPresentation)
{
  SessionItem item;
  item.SetData(42);

  const std::string expected_label("abc");
  LabelPresentationItem presentation(&item, expected_label);

  // item has a display role, which coincide with the label, the rest is blocked
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString().toStdString(), expected_label);
  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());

  EXPECT_FALSE(presentation.SetData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(presentation.SetData(QString("bbb"), Qt::EditRole));

  // data is the same as before, despite of all attempts to change it
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString().toStdString(), expected_label);
}

TEST_F(StandardPresentationItemsTest, EditableDisplayNamePresentationItem)
{
  SessionItem item;
  item.SetDisplayName("abc");

  EditableDisplayNamePresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetDataRole(), DataRole::kDisplay);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());

  EXPECT_TRUE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_TRUE(presentation.Data(Qt::DisplayRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::EditRole).toString(), QString("abc"));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("abc"));

  EXPECT_TRUE(presentation.SetData("abcabc", Qt::EditRole));
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::DisplayRole));

  EXPECT_EQ(item.GetDisplayName(), std::string("abcabc"));

  // it is not possible to set same data twice
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::EditRole));
}

//! The behavior of editable display item when no initial name was set (real-life bug).

TEST_F(StandardPresentationItemsTest, EditableDisplayNamePresentationItemWithoutInitialSet)
{
  SessionItem item;
  // We do not set item, expect default display name "SessionItem"
  // item.SetDisplayName("abc");

  EditableDisplayNamePresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetDataRole(), DataRole::kDisplay);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());

  EXPECT_TRUE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_TRUE(presentation.Data(Qt::DisplayRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::EditRole).toString(), QString::fromStdString(SessionItem::Type));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(),
            QString::fromStdString(SessionItem::Type));

  EXPECT_TRUE(presentation.SetData("abcabc", Qt::EditRole));
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::DisplayRole));

  EXPECT_EQ(item.GetDisplayName(), std::string("abcabc"));

  // it is not possible to set same data twice
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::EditRole));
}

//! The behavior of editable display item when showing an item with bool data on bopard.
//! There shouldn't be any CheckState role visible (real-life bug).

TEST_F(StandardPresentationItemsTest, EditableDisplayNamePresentationItemForBoolean)
{
  SessionItem item;
  // We do not set item, expect default display name "SessionItem"
  item.SetData(true);
  item.SetDisplayName("abc");

  EditableDisplayNamePresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetDataRole(), DataRole::kDisplay);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());

  // thre shouldn't be any check state role
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());

  EXPECT_TRUE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_TRUE(presentation.Data(Qt::DisplayRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::EditRole).toString(), QString("abc"));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("abc"));

  EXPECT_TRUE(presentation.SetData("abcabc", Qt::EditRole));
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::DisplayRole));

  EXPECT_EQ(item.GetDisplayName(), std::string("abcabc"));
}
