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

#include "mvvm/viewmodel/viewmodelutils.h"

#include "mvvm/model/mvvm_types.h"

#include <gtest/gtest.h>

using namespace mvvm;

class ViewModelUtilsTest : public ::testing::Test
{
public:
};

//! Validate Utils::iterate_model function with user callback.

TEST_F(ViewModelUtilsTest, ItemRoleToQtRole)
{
  // DATA role of SessionItem should be translated to two Qt roles (edit and display)
  auto roles = Utils::ItemRoleToQtRole(DataRole::kData);
  QVector<int> expected = {Qt::DisplayRole, Qt::EditRole};
  EXPECT_EQ(roles, expected);

  // Appearance roles of SessionItem on Qt site means color
  roles = Utils::ItemRoleToQtRole(DataRole::kAppearance);
  expected = {Qt::ForegroundRole};
  EXPECT_EQ(roles, expected);

  // tooltip role
  roles = Utils::ItemRoleToQtRole(DataRole::kTooltip);
  expected = {Qt::ToolTipRole};
  EXPECT_EQ(roles, expected);
}
