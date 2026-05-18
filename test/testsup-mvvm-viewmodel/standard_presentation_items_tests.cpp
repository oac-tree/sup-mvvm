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

#include "mvvm/providers/standard_presentation_items.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/standarditems/editor_constants.h>

#include <gtest/gtest.h>

#include <QColor>

namespace mvvm::test
{

/**
 * @brief Tests for classes defined in standard_presentation_items.h
 */
class StandardPresentationItemsTest : public ::testing::Test
{
};

TEST_F(StandardPresentationItemsTest, CreateLabelPresentation)
{
  SessionItem item;
  item.SetData(42);

  const std::string expected_label("abc");
  LabelPresentationItem presentation(&item, expected_label);

  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDisplay).isEmpty());

  EXPECT_FALSE(presentation.IsEditable());

  // item has a display role, which coincide with the label, the rest is blocked
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString().toStdString(), expected_label);
  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());

  EXPECT_FALSE(presentation.SetData(QString("aaa"), Qt::DisplayRole));
  EXPECT_FALSE(presentation.SetData(QString("bbb"), Qt::EditRole));

  // data is the same as before, despite of all attempts to change it
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString().toStdString(), expected_label);
}

TEST_F(StandardPresentationItemsTest, FixedDataPresentationItem)
{
  SessionItem item;
  item.SetData(42);

  {  // no data on board
    FixedDataPresentationItem presentation(&item, {});
    EXPECT_EQ(presentation.GetItem(), &item);
    EXPECT_FALSE(presentation.Data(Qt::DisplayRole).isValid());
    EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDisplay).empty());
    EXPECT_TRUE(presentation.IsEnabled());
    EXPECT_FALSE(presentation.IsEditable());
  }

  {  // label on board
    const QString expected_label("abc");
    FixedDataPresentationItem presentation(&item, {{Qt::DisplayRole, QVariant(expected_label)}});
    EXPECT_EQ(presentation.GetItem(), &item);
    EXPECT_TRUE(presentation.Data(Qt::DisplayRole).isValid());
    // fixed data doesn't depend on SessionItem's data role, so we do not expect any notifications
    EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDisplay).empty());
    EXPECT_TRUE(presentation.IsEnabled());
    EXPECT_FALSE(presentation.IsEditable());

    // item has a display role, which coincide with the label, the rest is blocked
    EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), expected_label);
    EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
    EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

    // setting both roles, internal data should be updated
    EXPECT_TRUE(presentation.SetData(QString("aaa"), Qt::DisplayRole));
    EXPECT_TRUE(presentation.SetData(QString("bbb"), Qt::EditRole));

    EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("aaa"));
    EXPECT_EQ(presentation.Data(Qt::EditRole).toString(), QString("bbb"));
    // fixed data doesn't depend on SessionItem's data role, so we do not expect any notifications
    EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDisplay).empty());
  }

  {  // label on board, when item is disabled, and there is no explicit ForegroundRoles
    SessionItem item;
    item.SetData(42);
    item.SetEnabled(false);

    const QString expected_label("abc");
    FixedDataPresentationItem presentation(&item, {{Qt::DisplayRole, QVariant(expected_label)}});
    EXPECT_EQ(presentation.GetItem(), &item);
    EXPECT_TRUE(presentation.Data(Qt::DisplayRole).isValid());
    // fixed data doesn't depend on SessionItem's data role, so we do not expect any notifications
    EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDisplay).empty());
    EXPECT_FALSE(presentation.IsEnabled());
    EXPECT_FALSE(presentation.IsEditable());

    // item has a display role, which coincide with the label
    EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), expected_label);
    EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());

    // Our current FixedDataPresentationItem implementation should report ForegroundRole as from
    // parent SessionItemPresentation (gray color corresponding to disabled item).
    EXPECT_TRUE(presentation.Data(Qt::ForegroundRole).isValid());
    EXPECT_EQ(presentation.Data(Qt::ForegroundRole), QColor(Qt::gray));

    // however, if we set foreground role by ourself, it should override "gray" color of disabled
    // item.
    EXPECT_TRUE(presentation.SetData(QVariant::fromValue(QColor(Qt::red)), Qt::ForegroundRole));
    EXPECT_EQ(presentation.Data(Qt::ForegroundRole), QColor(Qt::red));
  }
}

}  // namespace mvvm::test
