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

#include "mvvm/editors/nullable_bool_editor.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QComboBox>
#include <QSignalSpy>

namespace mvvm::test
{

//! Tests for NullableBoolEditor class.

class NullableBoolEditorTest : public ::testing::Test
{
};

TEST_F(NullableBoolEditorTest, InitialState)
{
  NullableBoolEditor editor;
  EXPECT_FALSE(editor.GetData().isValid());
  EXPECT_FALSE(editor.IsPersistent());
  EXPECT_EQ(editor.GetPlaceholderText(), QString("default"));

  auto combo = editor.GetComboBox();
  ASSERT_EQ(combo->count(), 3);
  EXPECT_EQ(combo->itemText(1), QString("True"));
  EXPECT_EQ(combo->itemText(2), QString("False"));
}

TEST_F(NullableBoolEditorTest, SetData)
{
  NullableBoolEditor editor;
  QSignalSpy spy_data_changed(&editor, &NullableBoolEditor::dataChanged);

  editor.SetData(QVariant(true));
  EXPECT_EQ(editor.GetComboBox()->currentIndex(), 1);

  editor.SetData(QVariant(false));
  EXPECT_EQ(editor.GetComboBox()->currentIndex(), 2);

  editor.SetData(QVariant());
  EXPECT_EQ(editor.GetComboBox()->currentIndex(), 0);

  // no signals when the data is set from outside
  EXPECT_EQ(spy_data_changed.count(), 0);

  EXPECT_THROW(editor.SetData(QVariant(42)), RuntimeException);
}

TEST_F(NullableBoolEditorTest, SelectEntry)
{
  NullableBoolEditor editor;
  editor.SetData(QVariant(true));
  QSignalSpy spy_data_changed(&editor, &NullableBoolEditor::dataChanged);

  editor.GetComboBox()->setCurrentIndex(2);
  EXPECT_EQ(mvvm::test::TakeValue<QVariant>(spy_data_changed), QVariant(false));
  EXPECT_EQ(editor.GetData(), QVariant(false));

  // the first entry removes the value
  editor.GetComboBox()->setCurrentIndex(0);
  EXPECT_FALSE(mvvm::test::TakeValue<QVariant>(spy_data_changed).isValid());
  EXPECT_FALSE(editor.GetData().isValid());
}

TEST_F(NullableBoolEditorTest, SetPlaceholderText)
{
  NullableBoolEditor editor;
  editor.SetPlaceholderText("auto");
  EXPECT_EQ(editor.GetPlaceholderText(), QString("auto"));
  EXPECT_EQ(editor.GetComboBox()->itemText(0), QString("auto"));
}

}  // namespace mvvm::test
