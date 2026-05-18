/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "mvvm/providers/default_display_name_presentation_item.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/providers/custom_variants.h>
#include <mvvm/standarditems/editor_constants.h>

#include <gtest/gtest.h>

#include <QColor>

namespace mvvm::test
{

/**
 * @brief Tests for DefaultDisplayNamePresentationItem class.
 */
class DefaultDisplayNamePresentationItemTest : public ::testing::Test
{
};

TEST_F(DefaultDisplayNamePresentationItemTest, NonEditableDisplayNamePresentation)
{
  SessionItem item;
  EXPECT_EQ(item.GetDisplayName(), mvvm::GetTypeName<SessionItem>());

  DefaultDisplayNamePresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_FALSE(presentation.IsEditable());
  EXPECT_FALSE(presentation.IsCheckable());
  EXPECT_EQ(presentation.GetMainDataRole(), DataRole::kDisplay);

  EXPECT_EQ(presentation.Data(Qt::EditRole).toString(), QString("SessionItem"));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("SessionItem"));

  // for the moment it is not possible to set display name from a view
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::EditRole));
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::DisplayRole));

  // changing display name directly on item should be reflected in presentation
  item.SetDisplayName("abc");
  EXPECT_EQ(presentation.Data(Qt::EditRole).toString(), QString("abc"));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("abc"));

  // roles to emit
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kDisplay),
            QVector<int>({Qt::DisplayRole, Qt::EditRole}));
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kData).isEmpty());

  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kAppearance),
            QVector<int>({Qt::ForegroundRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kTooltip), QVector<int>({Qt::ToolTipRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kTextColor),
            QVector<int>({Qt::ForegroundRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kCheckState),
            QVector<int>({Qt::CheckStateRole}));

  // other roles doesn't have Qt equivalent
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kIdentifier).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kEditor).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kLowerLimit).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kUpperLimit).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDataBackup).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kSelected).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kUser).isEmpty());
}

TEST_F(DefaultDisplayNamePresentationItemTest, NonEditableDisplayNameForNullItem)
{
  DefaultDisplayNamePresentationItem presentation(nullptr);
  EXPECT_EQ(presentation.GetItem(), nullptr);
  EXPECT_FALSE(presentation.IsEnabled());
  EXPECT_FALSE(presentation.IsEditable());
  EXPECT_FALSE(presentation.IsCheckable());
  EXPECT_EQ(presentation.GetMainDataRole(), DataRole::kDisplay);

  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::DisplayRole).isValid());
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::EditRole));
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::DisplayRole));
}

TEST_F(DefaultDisplayNamePresentationItemTest, EditableDisplayNamePresentation)
{
  SessionItem item;
  item.SetDisplayName("abc");
  item.SetFlag(mvvm::Appearance::kEditableDisplayName, true);

  DefaultDisplayNamePresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());
  EXPECT_FALSE(presentation.IsCheckable());
  EXPECT_EQ(presentation.GetMainDataRole(), DataRole::kDisplay);

  EXPECT_EQ(presentation.Data(Qt::EditRole).toString(), QString("abc"));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("abc"));

  EXPECT_TRUE(presentation.SetData("abcabc", Qt::EditRole));
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::DisplayRole));

  EXPECT_EQ(item.GetDisplayName(), std::string("abcabc"));

  // setting same name
  EXPECT_FALSE(presentation.SetData("abcabc", Qt::EditRole));

  // roles to emit
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kDisplay),
            QVector<int>({Qt::DisplayRole, Qt::EditRole}));
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kData).isEmpty());

  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kAppearance),
            QVector<int>({Qt::ForegroundRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kTooltip), QVector<int>({Qt::ToolTipRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kTextColor),
            QVector<int>({Qt::ForegroundRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kCheckState),
            QVector<int>({Qt::CheckStateRole}));

  // other roles doesn't have Qt equivalent
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kIdentifier).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kEditor).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kLowerLimit).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kUpperLimit).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDataBackup).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kSelected).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kUser).isEmpty());
}

TEST_F(DefaultDisplayNamePresentationItemTest, DisplayNamePresentationItemTooltipRole)
{
  SessionItem item;

  const DefaultDisplayNamePresentationItem presentation(&item);
  EXPECT_FALSE(presentation.Data(Qt::ToolTipRole).isValid());

  item.SetToolTip("abc");
  EXPECT_EQ(presentation.Data(Qt::ToolTipRole).toString(), QString("abc"));
}

TEST_F(DefaultDisplayNamePresentationItemTest, DisplayNamePresentationItemForegroundRole)
{
  SessionItem item;
  item.SetData(42);

  const DefaultDisplayNamePresentationItem presentation(&item);

  // Enabled item doesn't have valid foreground role.
  // This means that the color of text is left for a view to decide.
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  item.SetEnabled(false);
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole), QColor(Qt::gray));

  item.SetEnabled(true);
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  // Hidden item has normal color by default. It is up to the view to decide how to show "hidden"
  // item.
  item.SetVisible(false);
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());
}

TEST_F(DefaultDisplayNamePresentationItemTest, DisplayNamePresentationItemCheckStateRole)
{
  SessionItem item;
  item.SetData(false);
  const DefaultDisplayNamePresentationItem presentation(&item);

  // no CheckStateRole for a display name
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());
}

//! The behavior of editable display item when showing an item with bool data on board.
//! There shouldn't be any CheckState role visible (real-life bug).
TEST_F(DefaultDisplayNamePresentationItemTest, EditableDisplayNamePresentationItemForBoolean)
{
  SessionItem item;
  // We do not set item, expect default display name "SessionItem"
  item.SetData(true);
  item.SetDisplayName("abc");
  item.SetFlag(Appearance::kEditableDisplayName, true);

  DefaultDisplayNamePresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetMainDataRole(), DataRole::kDisplay);
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

TEST_F(DefaultDisplayNamePresentationItemTest, EditableDisplayNamePresentationForReadOnlyMode)
{
  {  // kEditable
    SessionItem item;
    item.SetDisplayName("abc");
    // setting all possible flags as readonly
    item.SetFlag(mvvm::Appearance::kEditableDisplayName, false);
    item.SetEditable(false);

    // but explicitely setting presentation to editable mode
    const DefaultDisplayNamePresentationItem presentation(
        &item, DefaultDisplayNamePresentationItem::DisplayNameEditMode::kEditable);
    // item should be editable
    EXPECT_TRUE(presentation.IsEditable());
  }

  {  // kReadOnly
    SessionItem item;
    item.SetDisplayName("abc");
    // setting all possible flags as editable
    item.SetFlag(mvvm::Appearance::kEditableDisplayName, true);
    item.SetEditable(true);

    // but explicitely setting presentation to readonly mode
    const DefaultDisplayNamePresentationItem presentation(
        &item, DefaultDisplayNamePresentationItem::DisplayNameEditMode::kReadOnly);
    // item should be readonly
    EXPECT_FALSE(presentation.IsEditable());
  }
}

}  // namespace mvvm::test
