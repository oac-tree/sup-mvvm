/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include <mvvm/model/session_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#ifdef BUILD_QCUSTOMPLOT
#include <qcustomplot.h>
#endif

#include <QApplication>
#include <QMetaType>

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  QApplication app(argc, argv);
  Q_UNUSED(app)

  qRegisterMetaType<mvvm::SessionItem*>("mvvm::SessionItem*");
#ifdef BUILD_QCUSTOMPLOT
  qRegisterMetaType<QCPRange>("QCPRange");
#endif

  // run all google tests
  return RUN_ALL_TESTS();
}
