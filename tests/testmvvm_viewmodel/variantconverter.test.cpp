/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/variantconverter.h"

#include "test_utils.h"

#include <gtest/gtest.h>

#include <QColor>

using namespace mvvm;

//! Tests for utilities related to variant conversion.

class VariantConverterTest : public ::testing::Test
{
};

TEST_F(VariantConverterTest, GetStdVariant)
{
  EXPECT_EQ(GetStdVariant(QVariant()), variant_t());
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(true)), variant_t(true));
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(42)), variant_t(42));
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(1.0)), variant_t(1.0));
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(QString("abc"))), variant_t(std::string("abc")));
  const std::vector<double> vec{1.0, 2.0};
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(vec)), variant_t(vec));

  const ComboProperty combo = ComboProperty::CreateFrom({"a1"});
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(combo)), variant_t(combo));

  // other Qt variants are unsupported
  EXPECT_THROW(GetStdVariant(QVariant(QColor(Qt::red))), std::runtime_error);
}

TEST_F(VariantConverterTest, GetQtVariant)
{
  // from undefined variant
  EXPECT_EQ(GetQtVariant(variant_t()), QVariant());

  // from bool
  EXPECT_EQ(GetQtVariant(variant_t(true)), QVariant::fromValue(true));
  EXPECT_EQ(GetQtVariant(variant_t(false)), QVariant::fromValue(false));

  // from int
  EXPECT_EQ(GetQtVariant(variant_t(42)), QVariant::fromValue(42));

  // from double
  EXPECT_EQ(GetQtVariant(variant_t(1.1)), QVariant::fromValue(1.1));

  // from std::vector<double>
  const std::vector<double> vec({1.0, 2.0});
  auto mvvm_variant = variant_t(vec);
  // getting qt variant via conversion
  auto qt_variant = GetQtVariant(mvvm_variant);
  // getting vectors from variants and checking them
  auto vec_from_qt = qt_variant.value<std::vector<double>>();
  auto vec_from_mvvm = std::get<std::vector<double>>(mvvm_variant);
  EXPECT_EQ(vec_from_qt, vec);
  EXPECT_EQ(vec_from_mvvm, vec);

  // from std::string to QString
  const std::string str{"abc"};
  variant_t mvvm_str_variant(str);
  QVariant qt_str_variant(QString::fromStdString(str));
  EXPECT_EQ(qt_str_variant, GetQtVariant(mvvm_str_variant));

  // from std::vector<double>
  const ComboProperty combo = ComboProperty::CreateFrom({"a1", "a2"});
  auto mvvm_combo_variant = variant_t(combo);
  // getting qt variant via conversion
  auto qt_combo_variant = GetQtVariant(mvvm_combo_variant);
  // getting vectors from variants and checking them
  auto combo_from_qt = qt_combo_variant.value<ComboProperty>();
  auto combo_from_mvvm = std::get<ComboProperty>(mvvm_combo_variant);
  EXPECT_EQ(combo_from_qt, combo);
  EXPECT_EQ(combo_from_mvvm, combo);
}

//! Special test for vector<double> in Qt variants.

TEST_F(VariantConverterTest, QVariantForDoubleVector)
{
  const std::vector<double> vec({1.0, 2.0});
  QVariant variant1 = QVariant::fromValue(vec);
  QVariant variant2 = QVariant::fromValue(vec);

  // Direct variant comparison requires comparator registration.
  // We are not comparing those in normal application.
  QMetaType::registerComparators<std::vector<double>>();
  EXPECT_EQ(variant1, variant2);

  // however vectors in them are Ok
  EXPECT_EQ(variant1.value<std::vector<double>>(), variant2.value<std::vector<double>>());
}

//! Special test for ComboProperty in Qt variants
//! There is some unclear behavior in Qt when two variants constructed in the same way
//! are not equal to each other.

TEST_F(VariantConverterTest, QVariantForComboProperty)
{
  const ComboProperty combo = ComboProperty::CreateFrom({"a1", "a2"});
  QVariant variant1 = QVariant::fromValue(combo);
  QVariant variant2 = QVariant::fromValue(combo);

  // Direct variant comparison requires comparator registration.
  // We are not comparing those in normal application.
  QMetaType::registerComparators<ComboProperty>();
  EXPECT_EQ(variant1, variant2);

  // however vectors in them are Ok
  EXPECT_EQ(variant1.value<ComboProperty>(), variant2.value<ComboProperty>());
}
