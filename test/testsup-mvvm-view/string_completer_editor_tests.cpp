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

#include "mvvm/editors/string_completer_editor.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QCompleter>
#include <QLineEdit>
#include <QSignalSpy>
#include <QStringListModel>
#include <QTest>

using namespace mvvm;

/**
 * @brief Tests for StringCompleterEditor class.
 */
class StringCompleterEditorTest : public ::testing::Test
{
};

TEST_F(StringCompleterEditorTest, InitialState)
{
  EXPECT_THROW(StringCompleterEditor({}, nullptr), RuntimeException);

  const StringCompleterEditor editor([]() -> QStringList { return {}; });
  EXPECT_TRUE(editor.GetLineEdit()->text().isEmpty());
  EXPECT_FALSE(editor.value().isValid());
  EXPECT_EQ(editor.GetCompleter()->model()->rowCount(), 0);
  EXPECT_EQ(editor.GetCompleter()->model()->columnCount(), 1);
}

TEST_F(StringCompleterEditorTest, InitialStateWHenCallbackDefined)
{
  const QStringList options({"ABC", "ABC-DEF"});
  auto get_string_list_func = [options]() { return options; };

  StringCompleterEditor editor(get_string_list_func);
  EXPECT_TRUE(editor.GetLineEdit()->text().isEmpty());
  EXPECT_FALSE(editor.value().isValid());
  EXPECT_EQ(editor.GetCompleter()->model()->rowCount(), 2);
  EXPECT_EQ(editor.GetCompleter()->model()->columnCount(), 1);
}

TEST_F(StringCompleterEditorTest, SetValue)
{
  const QStringList options({"ABC", "ABC-DEF"});
  auto get_string_list_func = [options]() { return options; };

  StringCompleterEditor editor(get_string_list_func);

  QSignalSpy spy_value_changed(&editor, &StringCompleterEditor::valueChanged);

  const QString str_value("hello");
  editor.setValue(QVariant::fromValue(str_value));

  EXPECT_EQ(editor.GetLineEdit()->text(), str_value);
  EXPECT_EQ(editor.value(), QVariant::fromValue(str_value));
  ASSERT_EQ(spy_value_changed.count(), 1);

  EXPECT_EQ(mvvm::test::GetSendItem<QVariant>(spy_value_changed), QVariant::fromValue(str_value));

  // setting same value again
  editor.setValue(QVariant::fromValue(str_value));
  EXPECT_EQ(editor.value(), QVariant::fromValue(str_value));
  ASSERT_EQ(spy_value_changed.count(), 0);
}

//! Validating that completer model is updated at the moment of focus event.
TEST_F(StringCompleterEditorTest, SetFocus)
{
  QStringList options({"ABC", "ABC-DEF"});
  auto get_string_list_func = [&options]() { return options; };

  StringCompleterEditor editor(get_string_list_func);

  EXPECT_EQ(editor.GetCompleter()->model()->rowCount(), 2);

  // modifying options
  options = QStringList({"AAA"});

  // the only working way to test focus event
  // QTest::mousePress(editor.GetLineEdit(), Qt::LeftButton, Qt::NoModifier);
  // editor.GetLineEdit()->setFocus(Qt::OtherFocusReason);
  QFocusEvent* focus_event = new QFocusEvent(QEvent::FocusIn, Qt::OtherFocusReason);
  QCoreApplication::postEvent(editor.GetLineEdit(), focus_event);
  QCoreApplication::processEvents();

  EXPECT_EQ(editor.GetCompleter()->model()->rowCount(), 1);
}
