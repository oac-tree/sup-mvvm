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

#include "mvvm/editors/scientific_spinbox.h"

#include <mvvm/providers/variant_converter.h>
#include <mvvm/test/test_helper.h>
#include <mvvm/utils/i_limited_float.h>
#include <mvvm/utils/limited_float_helper.h>

#include <gtest/gtest.h>

#include <QApplication>
#include <QFocusEvent>
#include <QSignalSpy>
#include <QTest>

namespace mvvm::test
{

//! Tests for ScientificSpinBox class.

class ScientificSpinBoxTest : public ::testing::Test
{
public:
  //! Replaces the text of the editor with the given text, as if typed by the user.
  static void TypeText(ScientificSpinBox& editor, const QString& text)
  {
    editor.selectAll();
    QTest::keyClick(&editor, Qt::Key_Delete);
    QTest::keyClicks(&editor, text);
  }
};

TEST_F(ScientificSpinBoxTest, InitialState)
{
  ScientificSpinBox editor;
  EXPECT_EQ(editor.GetLimitedFloat(), nullptr);
  EXPECT_FALSE(editor.value().isValid());
  EXPECT_TRUE(editor.text().isEmpty());
  EXPECT_EQ(editor.GetPrecision(), 4);
  EXPECT_FALSE(editor.IsScientific());
  EXPECT_EQ(editor.GetSingleStep(), 1.0);
}

TEST_F(ScientificSpinBoxTest, SetLimitedFloat)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  auto limited_float = CreateLimitedFloat(variant_t{42.123456}, {}, {});
  auto limited_float_ptr = limited_float.get();
  editor.SetLimitedFloat(std::move(limited_float));
  EXPECT_EQ(limited_float_ptr, editor.GetLimitedFloat());

  // there shouldn't be any signals on value set
  EXPECT_EQ(spy_value_changed.count(), 0);

  EXPECT_EQ(editor.text(), QString("42.1235"));
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{42.123456});
}

TEST_F(ScientificSpinBoxTest, PrecisionAndNotation)
{
  ScientificSpinBox editor;
  editor.SetLimitedFloat(CreateLimitedFloat(variant_t{42.123456}, {}, {}));

  editor.SetPrecision(2);
  EXPECT_EQ(editor.text(), QString("42.12"));

  editor.SetScientific(true);
  EXPECT_EQ(editor.text(), QString("4.21e+01"));

  // the value is not rounded
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{42.123456});
}

TEST_F(ScientificSpinBoxTest, SetValue)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.SetLimitedFloat(CreateLimitedFloat(variant_t{42.0}, {}, {}));

  // setting the same value, no signals are expected
  editor.setValue(QVariant(42.0));
  EXPECT_EQ(spy_value_changed.count(), 0);

  // changing the value and expecting one signal
  editor.setValue(QVariant(43.5));
  EXPECT_EQ(editor.text(), QString("43.5000"));
  EXPECT_EQ(GetStdVariant(mvvm::test::TakeValue<QVariant>(spy_value_changed)), variant_t{43.5});
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{43.5});

  // after notification shouldn't be more signals than before
  editor.CheckNotify();
  EXPECT_EQ(spy_value_changed.count(), 0);
}

TEST_F(ScientificSpinBoxTest, SetValueWithoutLimitedFloat)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.SetRange(variant_t{float32{10.0}}, variant_t{float32{30.0}});
  editor.SetSingleStep(0.5);
  editor.setValue(GetQtVariant(variant_t{float32{20.0}}));

  // first value creates limited float without signals
  EXPECT_EQ(spy_value_changed.count(), 0);
  EXPECT_EQ(editor.text(), QString("20.0000"));

  ASSERT_NE(editor.GetLimitedFloat(), nullptr);
  EXPECT_EQ(editor.GetLimitedFloat()->GetLowerBoundAsVariant(), variant_t{float32{10.0}});
  EXPECT_EQ(editor.GetLimitedFloat()->GetUpperBoundAsVariant(), variant_t{float32{30.0}});
  EXPECT_EQ(editor.GetLimitedFloat()->GetSingleStep(), 0.5);

  // float32 stays float32
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{float32{20.0}});
}

TEST_F(ScientificSpinBoxTest, ValueReportsTypedText)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.SetLimitedFloat(CreateLimitedFloat(variant_t{0.1}, variant_t{-0.1}, variant_t{1e+7}));

  auto to_value = [&editor](const QString& text)
  {
    TypeText(editor, text);
    return GetStdVariant(editor.value());
  };

  // translation fails
  EXPECT_EQ(to_value("100.000.000.2"), variant_t{0.1});
  EXPECT_EQ(to_value("1e+2345"), variant_t{0.1});
  EXPECT_EQ(to_value("--0.1"), variant_t{0.1});
  EXPECT_EQ(to_value("-.e-12"), variant_t{0.1});
  EXPECT_EQ(to_value(""), variant_t{0.1});

  // translation fails due to out-of-bounds condition
  EXPECT_EQ(to_value("-0.2"), variant_t{0.1});
  EXPECT_EQ(to_value("1e+8"), variant_t{0.1});

  // legitimate values
  EXPECT_EQ(to_value("-0.0999"), variant_t{-0.0999});
  EXPECT_EQ(to_value("-.1e-12"), variant_t{-1e-13});
  EXPECT_EQ(to_value("1.1e+6"), variant_t{1.1e+6});
  EXPECT_EQ(to_value("0.123456789"), variant_t{0.123456789});

  // typed value is reported before editing finished
  EXPECT_EQ(editor.GetLimitedFloat()->GetValueAsVariant(), variant_t{0.1});
  EXPECT_EQ(spy_value_changed.count(), 0);
}

TEST_F(ScientificSpinBoxTest, InvalidCharactersAreRejected)
{
  ScientificSpinBox editor;
  editor.SetLimitedFloat(CreateLimitedFloat(variant_t{1.0}, {}, {}));

  TypeText(editor, "1a.5x");
  EXPECT_EQ(editor.text(), QString("1.5"));
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{1.5});

  // group separator is not accepted
  TypeText(editor, "1,2");
  EXPECT_EQ(editor.text(), QString("12"));
}

TEST_F(ScientificSpinBoxTest, EditingFinished)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.SetLimitedFloat(CreateLimitedFloat(variant_t{1.0}, {}, {}));

  TypeText(editor, "2.123456789");
  QTest::keyClick(&editor, Qt::Key_Return);
  EXPECT_EQ(editor.GetLimitedFloat()->GetValueAsVariant(), variant_t{2.123456789});
  EXPECT_EQ(editor.text(), QString("2.1235"));
  EXPECT_EQ(GetStdVariant(mvvm::test::TakeValue<QVariant>(spy_value_changed)),
            variant_t{2.123456789});

  // the text of the current value keeps the exact value
  QTest::keyClick(&editor, Qt::Key_Return);
  EXPECT_EQ(editor.GetLimitedFloat()->GetValueAsVariant(), variant_t{2.123456789});
  EXPECT_EQ(spy_value_changed.count(), 0);

  // invalid text is replaced with the text of the value
  TypeText(editor, "1e");
  QTest::keyClick(&editor, Qt::Key_Return);
  EXPECT_EQ(editor.GetLimitedFloat()->GetValueAsVariant(), variant_t{2.123456789});
  EXPECT_EQ(editor.text(), QString("2.1235"));
  EXPECT_EQ(spy_value_changed.count(), 0);
}

TEST_F(ScientificSpinBoxTest, StepBy)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.SetSingleStep(0.5);
  editor.SetLimitedFloat(CreateLimitedFloat(variant_t{1.0}, variant_t{0.0}, variant_t{2.0}));
  EXPECT_EQ(editor.GetLimitedFloat()->GetSingleStep(), 0.5);

  editor.stepBy(1);

  // there shouldn't be any signals on step
  EXPECT_EQ(spy_value_changed.count(), 0);
  EXPECT_EQ(editor.text(), QString("1.5000"));
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{1.5});

  // in editor this method is called on editing finished event
  editor.CheckNotify();
  EXPECT_EQ(GetStdVariant(mvvm::test::TakeValue<QVariant>(spy_value_changed)), variant_t{1.5});

  // forced into bounds
  editor.stepBy(4);
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{2.0});
  EXPECT_TRUE(editor.GetLimitedFloat()->IsAtMaximum());
}

TEST_F(ScientificSpinBoxTest, NullableWithoutValue)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.SetLimitedFloat(CreateNullableLimitedFloat({}, variant_t{42.0}, {}, {}));

  EXPECT_EQ(spy_value_changed.count(), 0);
  EXPECT_EQ(editor.text(), QString());
  EXPECT_FALSE(editor.value().isValid());
}

TEST_F(ScientificSpinBoxTest, NullableSetValue)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.SetLimitedFloat(CreateNullableLimitedFloat(variant_t{42.0}, variant_t{0.0}, {}, {}));
  EXPECT_EQ(editor.text(), QString("42.0000"));

  // invalid variant removes the value
  editor.setValue(QVariant());
  EXPECT_EQ(editor.text(), QString());
  EXPECT_FALSE(editor.value().isValid());
  EXPECT_FALSE(mvvm::test::TakeValue<QVariant>(spy_value_changed).isValid());

  // the value is back
  editor.setValue(QVariant(43.0));
  EXPECT_EQ(editor.text(), QString("43.0000"));
  EXPECT_EQ(GetStdVariant(mvvm::test::TakeValue<QVariant>(spy_value_changed)), variant_t{43.0});
}

TEST_F(ScientificSpinBoxTest, NullableStepFromNoValue)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.SetLimitedFloat(CreateNullableLimitedFloat({}, variant_t{42.0}, {}, {}));

  // the first step sets the default value
  editor.stepBy(1);
  EXPECT_EQ(editor.text(), QString("42.0000"));
  EXPECT_EQ(spy_value_changed.count(), 0);

  editor.CheckNotify();
  EXPECT_EQ(GetStdVariant(mvvm::test::TakeValue<QVariant>(spy_value_changed)), variant_t{42.0});
}

TEST_F(ScientificSpinBoxTest, NullableClearOnEnter)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.SetLimitedFloat(CreateNullableLimitedFloat(variant_t{42.0}, variant_t{0.0}, {}, {}));

  // the whole text is selected, delete clears it
  QTest::keyClick(&editor, Qt::Key_Delete);
  EXPECT_EQ(editor.text(), QString());

  // typed empty text is reported as no value before editing finished
  EXPECT_FALSE(editor.value().isValid());
  EXPECT_TRUE(editor.GetLimitedFloat()->HasValue());
  EXPECT_EQ(spy_value_changed.count(), 0);

  QTest::keyClick(&editor, Qt::Key_Return);
  EXPECT_FALSE(editor.GetLimitedFloat()->HasValue());
  EXPECT_FALSE(mvvm::test::TakeValue<QVariant>(spy_value_changed).isValid());
}

TEST_F(ScientificSpinBoxTest, NullableClearOnFocusOut)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.SetLimitedFloat(CreateNullableLimitedFloat(variant_t{42.0}, variant_t{0.0}, {}, {}));

  QTest::keyClick(&editor, Qt::Key_Delete);

  QFocusEvent focus_out(QEvent::FocusOut);
  QApplication::sendEvent(&editor, &focus_out);

  // empty text is not reverted
  EXPECT_EQ(editor.text(), QString());
  EXPECT_FALSE(editor.GetLimitedFloat()->HasValue());
  EXPECT_FALSE(mvvm::test::TakeValue<QVariant>(spy_value_changed).isValid());
}

TEST_F(ScientificSpinBoxTest, NonNullableEmptyText)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.SetLimitedFloat(CreateLimitedFloat(variant_t{42.0}, {}, {}));

  QTest::keyClick(&editor, Qt::Key_Delete);
  EXPECT_EQ(editor.text(), QString());

  // empty text is not a value, the last value is reported
  EXPECT_EQ(GetStdVariant(editor.value()), variant_t{42.0});

  QTest::keyClick(&editor, Qt::Key_Return);
  EXPECT_TRUE(editor.GetLimitedFloat()->HasValue());
  EXPECT_EQ(spy_value_changed.count(), 0);
}

}  // namespace mvvm::test
