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

#include <mvvm/model/sessionitem.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <qcustomplot.h>

#include <QApplication>
#include <QMetaType>

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  QApplication app(argc, argv);
  Q_UNUSED(app)

  qRegisterMetaType<mvvm::SessionItem*>("mvvm::SessionItem*");
  qRegisterMetaType<QCPRange>("QCPRange");

  // run all google tests
  return RUN_ALL_TESTS();
}
