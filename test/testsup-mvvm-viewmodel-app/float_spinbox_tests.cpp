/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/float_spinbox.h"

#include <mvvm/utils/limited_integer_helper.h>
#include <mvvm/viewmodel/custom_variants.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QDoubleSpinBox>
#include <QSignalSpy>
#include <QTest>

using namespace mvvm;

//! Tests for FloatSpinBox class.

class FloatSpinBoxTest : public ::testing::Test
{
};

//! Out of curiocity comparing variants
TEST_F(FloatSpinBoxTest, ComparingVariants)
{
  const mvvm::float32 value_float32(42.0);
  const QVariant variant_float32 = QVariant::fromValue(value_float32);

  const mvvm::float64 value_float64(42.0);
  const QVariant variant_float64 = QVariant::fromValue(value_float64);

  // good to know this
  EXPECT_EQ(variant_float32, variant_float64);
}

//! We expect that FloatSpinBox works for both QVariants, based on float and on double
TEST_F(FloatSpinBoxTest, SetFloat)
{
  // setting a double, should get double back
  {
    FloatSpinBox editor;

    const mvvm::float64 value(42.0);
    const QVariant variant = QVariant::fromValue(value);
    EXPECT_EQ(utils::GetQtVariantName(variant), constants::kFloat64QtTypeName);

    editor.setValue(variant);

    // changing the value via underlying QDoubleSpinBox

    EXPECT_EQ(editor.value(), variant);
    EXPECT_EQ(utils::GetQtVariantName(editor.value()), constants::kFloat64QtTypeName);
  }

  // setting a float, should get float back
  {
    FloatSpinBox editor;

    const mvvm::float32 value(42.0);
    const QVariant variant = QVariant::fromValue(value);
    EXPECT_EQ(utils::GetQtVariantName(variant), constants::kFloat32QtTypeName);

    editor.setValue(variant);

    EXPECT_EQ(editor.value(), variant);
    EXPECT_EQ(utils::GetQtVariantName(editor.value()), constants::kFloat32QtTypeName);
  }
}

TEST_F(FloatSpinBoxTest, SetValueForFloat64)
{
  FloatSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &FloatSpinBox::valueChanged);

  const mvvm::float64 value(42.0);
  const QVariant variant = QVariant::fromValue(value);
  EXPECT_EQ(utils::GetQtVariantName(variant), constants::kFloat64QtTypeName);

  editor.setValue(variant);

  EXPECT_EQ(spy_value_changed.count(), 0);

  // changing the data via underlying spinbox
  auto double_spinbox = editor.findChild<QDoubleSpinBox*>();
  ASSERT_TRUE(double_spinbox);

  double_spinbox->setValue(43.0);

  EXPECT_EQ(mvvm::test::GetSendItem<QVariant>(spy_value_changed), QVariant{43.0});
}

TEST_F(FloatSpinBoxTest, SetValueForFloat32)
{
  FloatSpinBox editor;
  QSignalSpy spy_value_changed(&editor, &FloatSpinBox::valueChanged);

  const mvvm::float32 value(42.0);
  const QVariant initial_variant = QVariant::fromValue(value);
  EXPECT_EQ(utils::GetQtVariantName(initial_variant), constants::kFloat32QtTypeName);

  editor.setValue(initial_variant);

  EXPECT_EQ(spy_value_changed.count(), 0);

  // changing the data via underlying spinbox
  auto double_spinbox = editor.findChild<QDoubleSpinBox*>();
  ASSERT_TRUE(double_spinbox);

  double_spinbox->setValue(43.0);

  auto reported_data = mvvm::test::GetSendItem<QVariant>(spy_value_changed);

  const mvvm::float32 new_value(43.0);
  const QVariant expected_variant = QVariant::fromValue(new_value);

  // reported value
  EXPECT_EQ(reported_data, expected_variant);

  EXPECT_EQ(utils::GetQtVariantName(reported_data), constants::kFloat32QtTypeName);

  // value on board of the editor
  EXPECT_EQ(editor.value(), expected_variant);
  EXPECT_EQ(utils::GetQtVariantName(editor.value()), constants::kFloat32QtTypeName);
}
