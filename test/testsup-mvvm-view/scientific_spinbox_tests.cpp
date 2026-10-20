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

#include <gtest/gtest.h>

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
  const ScientificSpinBox editor;
  EXPECT_EQ(editor.value(), 0.0);
  EXPECT_TRUE(editor.text().isEmpty());
  EXPECT_EQ(editor.decimals(), 4);
}

TEST_F(ScientificSpinBoxTest, SetValue)
{
  ScientificSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  editor.setValue(42.123456);
  EXPECT_EQ(editor.text(), QString("4.2123e+01"));
  EXPECT_EQ(editor.value(), 42.123456);
  EXPECT_EQ(spy_value_changed.count(), 1);

  // decimals change the text, not the value
  editor.setDecimals(2);
  EXPECT_EQ(editor.text(), QString("4.21e+01"));
  EXPECT_EQ(editor.value(), 42.123456);
  EXPECT_EQ(spy_value_changed.count(), 1);

  editor.setValue(1.5);
  EXPECT_EQ(editor.text(), QString("1.5e+00"));
  EXPECT_EQ(spy_value_changed.count(), 2);

  editor.setValue(1e-12);
  EXPECT_EQ(editor.text(), QString("1.0e-12"));
}

TEST_F(ScientificSpinBoxTest, ValueFromTypedText)
{
  ScientificSpinBox editor;
  editor.setMinimum(-0.1);
  editor.setMaximum(1e+7);
  editor.setValue(0.1);

  auto to_value = [&editor](const QString& text)
  {
    TypeText(editor, text);
    return editor.value();
  };

  // translation fails
  EXPECT_EQ(to_value("abcd"), 0.1);
  EXPECT_EQ(to_value("1,2"), 0.1);
  EXPECT_EQ(to_value("100,000,000.2"), 0.1);
  EXPECT_EQ(to_value("100.000.000.2"), 0.1);
  EXPECT_EQ(to_value("1e+2345"), 0.1);
  EXPECT_EQ(to_value("-1e+2345"), 0.1);
  EXPECT_EQ(to_value("--0.1"), 0.1);
  EXPECT_EQ(to_value("-.e-12"), 0.1);
  EXPECT_EQ(to_value(""), 0.1);

  // translation fails due to out-of-bounds condition
  EXPECT_EQ(to_value("-0.2"), 0.1);
  EXPECT_EQ(to_value("-0.1e+1"), 0.1);
  EXPECT_EQ(to_value("1e+8"), 0.1);

  // legitimate values
  EXPECT_EQ(to_value("-0.0999"), -0.0999);
  EXPECT_EQ(to_value("-.1e-12"), -1e-13);
  EXPECT_EQ(to_value("0"), 0.0);
  EXPECT_EQ(to_value("0.123"), 0.123);
  EXPECT_EQ(to_value("1e+6"), 1e+6);
  EXPECT_EQ(to_value("1.1e+6"), 1.1e+6);
  EXPECT_EQ(to_value("0.012"), 0.012);
  EXPECT_EQ(to_value("0.123456789"), 0.123456789);

  // typed text is not applied before editing finished
  EXPECT_EQ(editor.text(), QString("0.123456789"));
  editor.setValue(0.1);
  EXPECT_EQ(editor.value(), 0.1);
}

TEST_F(ScientificSpinBoxTest, EditingFinished)
{
  ScientificSpinBox editor;
  editor.setValue(1.0);
  QSignalSpy spy_value_changed(&editor, &ScientificSpinBox::valueChanged);

  TypeText(editor, "2.123456789");
  QTest::keyClick(&editor, Qt::Key_Return);
  EXPECT_EQ(editor.value(), 2.123456789);
  EXPECT_EQ(editor.text(), QString("2.1235e+00"));
  EXPECT_EQ(spy_value_changed.count(), 1);

  // unchanged text keeps the exact value
  QTest::keyClick(&editor, Qt::Key_Return);
  EXPECT_EQ(editor.value(), 2.123456789);
  EXPECT_EQ(spy_value_changed.count(), 1);

  // invalid text restores the text of the value
  TypeText(editor, "abc");
  QTest::keyClick(&editor, Qt::Key_Return);
  EXPECT_EQ(editor.value(), 2.123456789);
  EXPECT_EQ(editor.text(), QString("2.1235e+00"));
  EXPECT_EQ(spy_value_changed.count(), 1);
}

TEST_F(ScientificSpinBoxTest, StepBy)
{
  ScientificSpinBox editor;
  editor.setMaximum(2.0);
  editor.setValue(1.0);
  editor.setSingleStep(0.5);

  editor.stepBy(1);
  EXPECT_EQ(editor.value(), 1.5);

  editor.stepBy(-2);
  EXPECT_EQ(editor.value(), 0.5);

  // step outside of the range is ignored
  editor.stepBy(4);
  EXPECT_EQ(editor.value(), 0.5);
}

}  // namespace mvvm::test
