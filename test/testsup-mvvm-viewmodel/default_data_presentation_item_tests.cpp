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

#include "mvvm/providers/default_data_presentation_item.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/providers/custom_variants.h>

#include <gtest/gtest.h>

#include <QColor>

namespace mvvm::test
{

/**
 * @brief Tests for DefaultDataPresentationItem class.
 */
class DefaultDataPresentationItemTest : public ::testing::Test
{
};

TEST_F(DefaultDataPresentationItemTest, ItemWithIntAndDataRoles)
{
  const auto main_data_role = DataRole::kData;

  SessionItem item;
  item.SetData(42);

  DefaultDataPresentationItem presentation(&item, main_data_role);
  EXPECT_EQ(presentation.GetMainDataRole(), main_data_role);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());
  EXPECT_FALSE(presentation.IsCheckable());

  // data for various roles
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("42"));
  EXPECT_TRUE(utils::IsIntVariant(presentation.Data(Qt::EditRole)));
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::ToolTipRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());

  // adjusting item text color
  item.SetData("#ff0000", DataRole::kTextColor);
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole).value<QColor>(), QColor(Qt::red));

  // modifying item check state
  item.SetData(Qt::Checked, DataRole::kCheckState);
  EXPECT_TRUE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
}

}  // namespace mvvm::test
