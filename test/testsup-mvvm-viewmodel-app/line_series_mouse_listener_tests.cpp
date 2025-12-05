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

#include "mvvm/plotting/charts/line_series_mouse_listener.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/plotting/charts/qt_charts.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace mvvm::test
{

/**
 * @brief Tests for LineSeriesMouseListener class.
 */
class LineSeriesMouseListenerTest : public ::testing::Test
{
public:
  ::testing::MockFunction<LineSeriesMouseListener::ReportCallback> m_listener;
};

TEST_F(LineSeriesMouseListenerTest, AttemptToConstruct)
{
  EXPECT_THROW(LineSeriesMouseListener(nullptr, LineSeriesMouseListener::ReportCallback{}),
               RuntimeException);
}

TEST_F(LineSeriesMouseListenerTest, ValidateCallbacksOnPressedAndReleased)
{
  QLineSeries line_series;
  line_series << QPointF(0.0, 0.0) << QPointF(1.0, 1.0) << QPointF(2.0, 2.0);

  LineSeriesMouseListener listener(&line_series, m_listener.AsStdFunction());

  EXPECT_CALL(m_listener, Call(&line_series, 1)).Times(1);
  emit line_series.pressed(QPointF(1.1, 1.1));

  EXPECT_CALL(m_listener, Call(&line_series, -1)).Times(1);
  emit line_series.released(QPointF(1.1, 1.1));
}

}  // namespace mvvm::test
