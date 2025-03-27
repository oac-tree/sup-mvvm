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

#include <gtest/gtest.h>

#include <QLineEdit>

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

  StringCompleterEditor editor([]() -> QStringList { return {}; });
  EXPECT_TRUE(editor.GetLineEdit()->text().isEmpty());
  EXPECT_FALSE(editor.value().isValid());
}

TEST_F(StringCompleterEditorTest, InitialStateWHenCallbackDefined)
{
  const QStringList options({"ABC", "ABC-DEF"});
  auto get_string_list_func = [&options]() { return options; };

  StringCompleterEditor editor(get_string_list_func);
  EXPECT_TRUE(editor.GetLineEdit()->text().isEmpty());
  EXPECT_FALSE(editor.value().isValid());
}
