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

#include "mvvm/editors/allint_spinbox.h"
#include "mvvm/providers/variant_converter.h"

#include <mvvm/test/test_helper.h>
#include <mvvm/utils/i_limited_integer.h>
#include <mvvm/utils/limited_integer_helper.h>

#include <gtest/gtest.h>

#include <QApplication>
#include <QFocusEvent>
#include <QSignalSpy>
#include <QTest>

namespace mvvm::test
{

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

  auto reported_data = mvvm::test::TakeValue<QVariant>(spy_value_changed);
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

  auto reported_data = mvvm::test::TakeValue<QVariant>(spy_value_changed);
  EXPECT_EQ(GetStdVariant(reported_data), variant_t{43});

  auto qt_variant = editor.value();
  EXPECT_EQ(GetStdVariant(qt_variant), variant_t{43});

  // after notification shouldn't be more signals than before
  editor.CheckNotify();
  EXPECT_EQ(spy_value_changed.count(), 0);
}

TEST_F(AllIntSpinBoxTest, SetValueWithoutLimitedInteger)
{
  AllIntSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &AllIntSpinBox::valueChanged);

  editor.SetRange(variant_t{int8{10}}, variant_t{int8{30}});
  editor.setValue(GetQtVariant(variant_t{int8{20}}));

  // first value creates limited integer without signals
  EXPECT_EQ(spy_value_changed.count(), 0);
  EXPECT_EQ(editor.text(), QString("20"));

  ASSERT_NE(editor.GetLimitedInteger(), nullptr);
  EXPECT_EQ(editor.GetLimitedInteger()->GetLowerBoundAsVariant(), variant_t{int8{10}});
  EXPECT_EQ(editor.GetLimitedInteger()->GetUpperBoundAsVariant(), variant_t{int8{30}});
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{int8{20}});
}

TEST_F(AllIntSpinBoxTest, ValueReportsTypedText)
{
  AllIntSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &AllIntSpinBox::valueChanged);

  editor.SetLimitedInteger(CreateLimitedInteger(variant_t{42}, variant_t{0}, variant_t{100}));

  // the whole text is selected, typing replaces it
  QTest::keyClicks(&editor, "25");
  EXPECT_EQ(editor.text(), QString("25"));

  // typed value is reported before editing finished
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{25});
  EXPECT_EQ(editor.GetLimitedInteger()->GetValueAsVariant(), variant_t{42});
  EXPECT_EQ(spy_value_changed.count(), 0);

  // out of range text reports the last value
  QTest::keyClicks(&editor, "0");
  EXPECT_EQ(editor.text(), QString("250"));
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{42});
}

TEST_F(AllIntSpinBoxTest, NullableWithoutValue)
{
  AllIntSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &AllIntSpinBox::valueChanged);

  editor.SetPlaceholderText("default");
  editor.SetLimitedInteger(CreateNullableLimitedInteger({}, variant_t{42}, {}, {}));

  EXPECT_EQ(spy_value_changed.count(), 0);
  EXPECT_EQ(editor.text(), QString());
  EXPECT_EQ(editor.GetPlaceholderText(), QString("default"));
  EXPECT_FALSE(editor.value().isValid());
}

TEST_F(AllIntSpinBoxTest, NullableSetValue)
{
  AllIntSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &AllIntSpinBox::valueChanged);

  editor.SetLimitedInteger(CreateNullableLimitedInteger(variant_t{42}, variant_t{0}, {}, {}));
  EXPECT_EQ(editor.text(), QString("42"));

  // invalid variant removes the value
  editor.setValue(QVariant());
  EXPECT_EQ(editor.text(), QString());
  EXPECT_FALSE(editor.value().isValid());
  EXPECT_FALSE(mvvm::test::TakeValue<QVariant>(spy_value_changed).isValid());

  // the value is back
  editor.setValue(QVariant(43));
  EXPECT_EQ(editor.text(), QString("43"));
  EXPECT_EQ(GetStdVariant(mvvm::test::TakeValue<QVariant>(spy_value_changed)), variant_t{43});
}

TEST_F(AllIntSpinBoxTest, NullableStepFromNoValue)
{
  AllIntSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &AllIntSpinBox::valueChanged);

  editor.SetLimitedInteger(CreateNullableLimitedInteger({}, variant_t{42}, {}, {}));

  // the first step sets the default value
  editor.stepBy(1);
  EXPECT_EQ(editor.text(), QString("42"));
  EXPECT_EQ(spy_value_changed.count(), 0);

  // in editor this method is called on editing finished event
  editor.CheckNotify();
  EXPECT_EQ(GetStdVariant(mvvm::test::TakeValue<QVariant>(spy_value_changed)), variant_t{42});
}

TEST_F(AllIntSpinBoxTest, NullableClearOnEnter)
{
  AllIntSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &AllIntSpinBox::valueChanged);

  editor.SetLimitedInteger(CreateNullableLimitedInteger(variant_t{42}, variant_t{0}, {}, {}));

  // the whole text is selected, delete clears it
  QTest::keyClick(&editor, Qt::Key_Delete);
  EXPECT_EQ(editor.text(), QString());

  // typed empty text is reported as no value before editing finished
  EXPECT_FALSE(editor.value().isValid());
  EXPECT_TRUE(editor.GetLimitedInteger()->HasValue());
  EXPECT_EQ(spy_value_changed.count(), 0);

  QTest::keyClick(&editor, Qt::Key_Return);
  EXPECT_FALSE(editor.GetLimitedInteger()->HasValue());
  EXPECT_FALSE(mvvm::test::TakeValue<QVariant>(spy_value_changed).isValid());
}

TEST_F(AllIntSpinBoxTest, NullableClearOnFocusOut)
{
  AllIntSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &AllIntSpinBox::valueChanged);

  editor.SetLimitedInteger(CreateNullableLimitedInteger(variant_t{42}, variant_t{0}, {}, {}));

  QTest::keyClick(&editor, Qt::Key_Delete);

  QFocusEvent focus_out(QEvent::FocusOut);
  QApplication::sendEvent(&editor, &focus_out);

  // empty text is not reverted
  EXPECT_EQ(editor.text(), QString());
  EXPECT_FALSE(editor.GetLimitedInteger()->HasValue());
  EXPECT_FALSE(mvvm::test::TakeValue<QVariant>(spy_value_changed).isValid());
}

TEST_F(AllIntSpinBoxTest, NonNullableEmptyText)
{
  AllIntSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &AllIntSpinBox::valueChanged);

  editor.SetLimitedInteger(CreateLimitedInteger(variant_t{42}, {}, {}));

  QTest::keyClick(&editor, Qt::Key_Delete);
  EXPECT_EQ(editor.text(), QString());

  // empty text is not a value, the last value is reported
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{42});

  QTest::keyClick(&editor, Qt::Key_Return);
  EXPECT_TRUE(editor.GetLimitedInteger()->HasValue());
  EXPECT_EQ(spy_value_changed.count(), 0);
}

}  // namespace mvvm::test
