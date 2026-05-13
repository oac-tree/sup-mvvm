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

#include "mvvm/providers/abstract_presentation_item.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>

#include <gtest/gtest.h>

namespace mvvm::test
{

namespace
{

/**
 * @brief Test implementation of AbstractItemPresentation.
 */
class TestPresentation : public AbstractItemPresentation
{
public:
  explicit TestPresentation(SessionItem* item) : AbstractItemPresentation(item) {};

  QVariant Data(role_t qt_role) const override
  {
    (void)qt_role;
    return {};
  };
  bool SetData(const QVariant& data, role_t qt_role) override
  {
    (void)data;
    (void)qt_role;
    return false;
  }
  bool IsCheckable() const override { return false; }
};

}  // namespace

/**
 * @brief Tests for AbstractPresentationItem class.
 */
class AbstractPresentationItemTest : public ::testing::Test
{
};

TEST_F(AbstractPresentationItemTest, TestPresentation)
{
  SessionItem item;

  item.SetEnabled(true);
  item.SetEditable(true);

  const TestPresentation presentation(&item);
  EXPECT_EQ(presentation.GetItem(), &item);

  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());
}

}  // namespace mvvm::test
