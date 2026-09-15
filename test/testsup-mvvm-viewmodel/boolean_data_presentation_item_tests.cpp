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

#include "mvvm/providers/boolean_data_presentation_item.h"

#include <mvvm/model/session_item.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for BooleanDataPresentationItem class.
 */
class BooleanDataPresentationItemTest : public ::testing::Test
{
};

TEST_F(BooleanDataPresentationItemTest, BooleanData)
{
  {  // true value as main data role
    SessionItem item;
    item.SetData(true);

    BooleanDataPresentationItem presentation(&item);
    EXPECT_EQ(presentation.GetMainDataRole(), DataRole::kData);
    EXPECT_TRUE(presentation.IsEnabled());
    EXPECT_FALSE(presentation.IsEditable());  // no editing on double click
    EXPECT_TRUE(presentation.IsCheckable());

    // main data role is available via edit role, even if the cell is not editable
    EXPECT_EQ(presentation.Data(Qt::EditRole), QVariant(true));

    EXPECT_EQ(presentation.Data(Qt::DisplayRole), QString("True"));
    EXPECT_TRUE(presentation.Data(Qt::CheckStateRole).isValid());
    EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Checked);

    EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDisplay).isEmpty());
    EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kData),
              QVector<int>({Qt::DisplayRole, Qt::EditRole, Qt::CheckStateRole}));
    EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kAppearance).isEmpty());

    // SetData
    EXPECT_TRUE(presentation.SetData(Qt::Unchecked, Qt::CheckStateRole));
    EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
    EXPECT_EQ(item.Data<bool>(), false);
    // same data again
    EXPECT_FALSE(presentation.SetData(Qt::Unchecked, Qt::CheckStateRole));

    EXPECT_TRUE(presentation.SetData(Qt::Checked, Qt::CheckStateRole));
    EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
    EXPECT_EQ(item.Data<bool>(), true);

    // acting through edit role
    EXPECT_TRUE(presentation.SetData(false, Qt::EditRole));
    EXPECT_EQ(item.Data<bool>(), false);
    EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
    // same data again
    EXPECT_FALSE(presentation.SetData(false, Qt::EditRole));

    // variant of the wrong type is rejected
    EXPECT_FALSE(presentation.SetData(QString("abc"), Qt::EditRole));
    EXPECT_EQ(item.Data<bool>(), false);
  }

  {  // false value as main data role
    SessionItem item;
    item.SetData(false);

    const BooleanDataPresentationItem presentation(&item);
    EXPECT_EQ(presentation.GetMainDataRole(), DataRole::kData);
    EXPECT_TRUE(presentation.IsEnabled());
    EXPECT_FALSE(presentation.IsEditable());  // no editing on double click
    EXPECT_TRUE(presentation.IsCheckable());

    // main data role is available via edit role, even if the cell is not editable
    EXPECT_EQ(presentation.Data(Qt::EditRole), QVariant(false));

    EXPECT_EQ(presentation.Data(Qt::DisplayRole), QString("False"));
    EXPECT_TRUE(presentation.Data(Qt::CheckStateRole).isValid());
    EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);

    EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDisplay).isEmpty());
    EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kData),
              QVector<int>({Qt::DisplayRole, Qt::EditRole, Qt::CheckStateRole}));
    EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kAppearance).isEmpty());
  }
}

TEST_F(BooleanDataPresentationItemTest, BooleanDataForCheckBoxOnlyAppearance)
{
  SessionItem item;
  item.SetData(true);

  BooleanDataPresentationItem presentation(
      &item, BooleanDataPresentationItem::AppearanceMode::kCheckBoxOnly);
  EXPECT_EQ(presentation.GetMainDataRole(), DataRole::kData);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_FALSE(presentation.IsEditable());  // no editing on double click
  EXPECT_TRUE(presentation.IsCheckable());

  EXPECT_FALSE(presentation.Data(Qt::DisplayRole).isValid());  // no text
  EXPECT_TRUE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Checked);

  // SetData
  EXPECT_TRUE(presentation.SetData(Qt::Unchecked, Qt::CheckStateRole));
  EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);
  EXPECT_EQ(item.Data<bool>(), false);
}

TEST_F(BooleanDataPresentationItemTest, BooleanDataForItemContainingInt)
{
  // We are using by mistake BooleanDataPresentationItem together with item carrying an integer.

  SessionItem item;
  item.SetData(42);

  const BooleanDataPresentationItem presentation(&item);
  EXPECT_EQ(presentation.GetMainDataRole(), DataRole::kData);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_FALSE(presentation.IsEditable());  // no editing on double click
  EXPECT_TRUE(presentation.IsCheckable());

  EXPECT_TRUE(presentation.Data(Qt::DisplayRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("42"));
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
}

}  // namespace mvvm::test
