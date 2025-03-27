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

#include "mvvm/editors/string_completer_combo_editor.h"

#include <mvvm/core/exceptions.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QComboBox>
#include <QSignalSpy>
#include <QTest>

using namespace mvvm;

/**
 * @brief Tests for StringCompleterComboEditor class.
 */
class StringCompleterComboEditorTest : public ::testing::Test
{
public:
  static QStringList GetStringList(QComboBox* combo)
  {
    QStringList result;
    for (int i = 0; i < combo->count(); ++i)
    {
      result.append(combo->itemText(i));
    }

    return result;
  }
};

TEST_F(StringCompleterComboEditorTest, InitialState)
{
  EXPECT_THROW(StringCompleterComboEditor({}, nullptr), RuntimeException);

  StringCompleterComboEditor editor([]() -> QStringList { return {}; });
  EXPECT_EQ(editor.GetComboBox()->currentIndex(), -1);
  EXPECT_EQ(editor.GetComboBox()->currentText(), QString());
  EXPECT_FALSE(editor.value().isValid());
}

TEST_F(StringCompleterComboEditorTest, InitialStateWHenCallbackDefined)
{
  const QStringList options({"ABC", "ABC-DEF"});
  // combo options contains empty string in front
  QStringList expected_combo_options = QStringList() << QString("") << options;
  auto get_string_list_func = [&options]() { return options; };

  StringCompleterComboEditor editor(get_string_list_func);
  EXPECT_EQ(editor.GetComboBox()->currentIndex(), 0);
  EXPECT_EQ(editor.GetComboBox()->currentText(), QString());
  EXPECT_FALSE(editor.value().isValid());

  EXPECT_EQ(GetStringList(editor.GetComboBox()), expected_combo_options);
}

TEST_F(StringCompleterComboEditorTest, SetValue)
{
  const QStringList options({"ABC", "ABC-DEF"});
  const QStringList expected_combo_options = QStringList() << QString("") << options;
  auto get_string_list_func = [&options]() { return options; };

  StringCompleterComboEditor editor(get_string_list_func);

  QSignalSpy spy_value_changed(&editor, &StringCompleterComboEditor::valueChanged);

  const QString str_value("hello");
  editor.setValue(QVariant::fromValue(str_value));

  EXPECT_EQ(editor.GetComboBox()->currentText(), str_value);
  EXPECT_EQ(editor.value(), QVariant::fromValue(str_value));
  EXPECT_EQ(GetStringList(editor.GetComboBox()), expected_combo_options);
  ASSERT_EQ(spy_value_changed.count(), 1);

  EXPECT_EQ(mvvm::test::GetSendItem<QVariant>(spy_value_changed), QVariant::fromValue(str_value));

  // setting same value again
  editor.setValue(QVariant::fromValue(str_value));
  EXPECT_EQ(editor.value(), QVariant::fromValue(str_value));
  EXPECT_EQ(GetStringList(editor.GetComboBox()), expected_combo_options);
  ASSERT_EQ(spy_value_changed.count(), 0);
}
