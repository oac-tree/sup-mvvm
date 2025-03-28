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
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>

using namespace mvvm;

/**
 * @brief Tests for StringCompleterComboEditor class.
 */
class StringCompleterComboEditorTest : public ::testing::Test
{
public:
  /**
   * @brief Returns string list made of all combo entry text.
   */
  static QStringList GetStringList(QComboBox* combo)
  {
    QStringList result;
    for (int i = 0; i < combo->count(); ++i)
    {
      result.append(combo->itemText(i));
    }

    return result;
  }

  /**
   * @brief Returns list made of combo entries for given completer options.
   *
   * To validate StringCompleterComboEditor behavior that always adds an empty entry at the
   * beginning.
   */
  static QStringList GetExpectedComboList(const QStringList& completer_options)
  {
    return QStringList() << QString("") << completer_options;
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

TEST_F(StringCompleterComboEditorTest, InitialStateWhenCallbackDefined)
{
  const QStringList completer_list({"ABC", "ABC-DEF"});
  auto get_completer_list_func = [&completer_list]() { return completer_list; };

  StringCompleterComboEditor editor(get_completer_list_func);
  EXPECT_EQ(editor.GetComboBox()->currentIndex(), 0);
  EXPECT_EQ(editor.GetComboBox()->currentText(), QString());
  EXPECT_FALSE(editor.value().isValid());

  EXPECT_EQ(GetStringList(editor.GetComboBox()), GetExpectedComboList(completer_list));
}

TEST_F(StringCompleterComboEditorTest, ComboBoxBehaviorOnSetValue)
{
  const QStringList completer_list({"ABC", "ABC-DEF"});
  auto get_string_list_func = [&completer_list]() { return completer_list; };

  StringCompleterComboEditor editor(get_string_list_func);

  QSignalSpy spy_value_changed(&editor, &StringCompleterComboEditor::valueChanged);

  const QString str_value("hello");
  editor.setValue(QVariant::fromValue(str_value));

  EXPECT_EQ(editor.GetComboBox()->currentText(), str_value);
  EXPECT_EQ(editor.value(), QVariant::fromValue(str_value));
  EXPECT_EQ(GetStringList(editor.GetComboBox()), GetExpectedComboList(completer_list));
  ASSERT_EQ(spy_value_changed.count(), 1);

  EXPECT_EQ(mvvm::test::GetSendItem<QVariant>(spy_value_changed), QVariant::fromValue(str_value));

  // setting same value again
  editor.setValue(QVariant::fromValue(str_value));
  EXPECT_EQ(editor.value(), QVariant::fromValue(str_value));
  EXPECT_EQ(GetStringList(editor.GetComboBox()), GetExpectedComboList(completer_list));
  ASSERT_EQ(spy_value_changed.count(), 0);
}

TEST_F(StringCompleterComboEditorTest, ComboBoxBehaviorOnSetIndex)
{
  const QStringList completer_list({"ABC", "ABC-DEF"});
  auto get_string_list_func = [&completer_list]() { return completer_list; };

  StringCompleterComboEditor editor(get_string_list_func);

  QSignalSpy spy_value_changed(&editor, &StringCompleterComboEditor::valueChanged);

  const int selected_combo_index{1};
  editor.GetComboBox()->setCurrentIndex(selected_combo_index);

  const QString expected_str_value{"ABC"};
  EXPECT_EQ(editor.GetComboBox()->currentText(), expected_str_value);
  EXPECT_EQ(editor.value(), QVariant::fromValue(expected_str_value));
  EXPECT_EQ(GetStringList(editor.GetComboBox()), GetExpectedComboList(completer_list));
  ASSERT_EQ(spy_value_changed.count(), 1);

  EXPECT_EQ(mvvm::test::GetSendItem<QVariant>(spy_value_changed),
            QVariant::fromValue(expected_str_value));
}

TEST_F(StringCompleterComboEditorTest, ComboBoxBehaviorOnEditingFinished)
{
  const QStringList completer_list({"ABC", "ABC-DEF"});
  auto get_string_list_func = [&completer_list]() { return completer_list; };

  StringCompleterComboEditor editor(get_string_list_func);

  QSignalSpy spy_value_changed(&editor, &StringCompleterComboEditor::valueChanged);

  const QString expected_str_value{"DEF"};
  editor.GetComboBox()->setEditText(expected_str_value);
  emit editor.GetComboBox()->lineEdit()->editingFinished();

  QCoreApplication::processEvents();

  EXPECT_EQ(editor.GetComboBox()->currentText(), expected_str_value);
  EXPECT_EQ(editor.value(), QVariant::fromValue(expected_str_value));
  EXPECT_EQ(GetStringList(editor.GetComboBox()), GetExpectedComboList(completer_list));
  ASSERT_EQ(spy_value_changed.count(), 1);

  EXPECT_EQ(mvvm::test::GetSendItem<QVariant>(spy_value_changed),
            QVariant::fromValue(expected_str_value));
}
