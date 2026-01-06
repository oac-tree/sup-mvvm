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

#include <mvvm/model/combo_property.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QMetaType>
#include <QPersistentModelIndex>

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);

  (void)qRegisterMetaType<QList<QPersistentModelIndex>>("QList<QPersistentModelIndex>");
  (void)qRegisterMetaType<QAbstractItemModel::LayoutChangeHint>(
      "QAbstractItemModel::LayoutChangeHint");

  // run all google tests
  return RUN_ALL_TESTS();
}
