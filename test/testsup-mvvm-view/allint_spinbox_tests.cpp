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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/allint_spinbox.h"
#include "mvvm/viewmodel/variant_converter.h"

#include <mvvm/test/test_helper.h>
#include <mvvm/utils/limited_integer_helper.h>

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTest>

using namespace mvvm;

//! Tests for AllIntSpinBox class.

class AllIntSpinBoxTest : public ::testing::Test
{
};

TEST_F(AllIntSpinBoxTest, SetInt32)
{
  AllIntSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &AllIntSpinBox::valueChanged);

  const variant_t initial_value{42};
  auto limited_int = CreateLimitedInteger(initial_value, {}, {});
  auto limited_int_ptr = limited_int.get();
  editor.SetLimitedInteger(std::move(limited_int));
  EXPECT_EQ(limited_int_ptr, editor.GetLimitedInteger());

  // there shouldn't be any signals on value set
  EXPECT_EQ(spy_value_changed.count(), 0);

  EXPECT_EQ(editor.text(), QString("42"));

  auto qt_variant = editor.value();
  EXPECT_EQ(spy_value_changed.count(), 0);

  EXPECT_EQ(GetStdVariant(qt_variant), initial_value);
}

TEST_F(AllIntSpinBoxTest, StepBy)
{
  AllIntSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &AllIntSpinBox::valueChanged);

  const variant_t initial_value{42};
  auto limited_int = CreateLimitedInteger(initial_value, {}, {});
  editor.SetLimitedInteger(std::move(limited_int));

  editor.stepBy(1);

  // there shouldn't be any signals on value set and on step
  EXPECT_EQ(spy_value_changed.count(), 0);

  EXPECT_EQ(editor.text(), QString("43"));

  auto qt_variant = editor.value();
  EXPECT_EQ(spy_value_changed.count(), 0);

  EXPECT_EQ(GetStdVariant(qt_variant), variant_t{43});

  // in editor this method is called on editing finished event
  editor.CheckNotify();

  auto reported_data = mvvm::test::GetSendItem<QVariant>(spy_value_changed);
  EXPECT_EQ(GetStdVariant(reported_data), variant_t{43});
}

TEST_F(AllIntSpinBoxTest, SetValue)
{
  AllIntSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &AllIntSpinBox::valueChanged);

  const variant_t initial_value{42};
  auto limited_int = CreateLimitedInteger(initial_value, {}, {});
  editor.SetLimitedInteger(std::move(limited_int));

  // setting the same value, no signals are expected
  editor.setValue(QVariant(42));
  EXPECT_EQ(spy_value_changed.count(), 0);

  // changing the value and expecting one signal
  editor.setValue(QVariant(43));
  EXPECT_EQ(spy_value_changed.count(), 1);

  EXPECT_EQ(editor.text(), QString("43"));

  auto reported_data = mvvm::test::GetSendItem<QVariant>(spy_value_changed);
  EXPECT_EQ(GetStdVariant(reported_data), variant_t{43});

  auto qt_variant = editor.value();
  EXPECT_EQ(GetStdVariant(qt_variant), variant_t{43});

  // after notification shouldn't be more signals than before
  editor.CheckNotify();
  EXPECT_EQ(spy_value_changed.count(), 0);
}
