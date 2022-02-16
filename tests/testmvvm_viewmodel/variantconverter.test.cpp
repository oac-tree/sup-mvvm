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

//! Testing function to convert Qt variant to std variant.

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

  const ExternalProperty external_property("text", "color", "identifier");
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(external_property)), variant_t(external_property));

  // other Qt variants are unsupported
  EXPECT_THROW(GetStdVariant(QVariant(QColor(Qt::red))), std::runtime_error);
}

//! Testing function to convert std variant to Qt variant.

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

  // from std::string to QString
  const std::string str{"abc"};
  variant_t mvvm_str_variant(str);
  QVariant qt_str_variant(QString::fromStdString(str));
  EXPECT_EQ(qt_str_variant, GetQtVariant(mvvm_str_variant));

  // This part is verbose because we are comparing underlying objects, and not Qt variants directly.
  // QVariant direct comparison requires comparators registration, which we want to avoid
  // in this test.

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

  // from ComboProperty
  const ComboProperty combo = ComboProperty::CreateFrom({"a1", "a2"});
  auto mvvm_combo_variant = variant_t(combo);
  // getting qt variant via conversion
  auto qt_combo_variant = GetQtVariant(mvvm_combo_variant);
  // getting ComboProperty from variants and checking them
  auto combo_from_qt = qt_combo_variant.value<ComboProperty>();
  auto combo_from_mvvm = std::get<ComboProperty>(mvvm_combo_variant);
  EXPECT_EQ(combo_from_qt, combo);
  EXPECT_EQ(combo_from_mvvm, combo);

  // from ExternalProperty
  const ExternalProperty external("text", "color", "identifier");
  auto mvvm_external_variant = variant_t(external);
  // getting qt variant via conversion
  auto qt_external_variant = GetQtVariant(mvvm_external_variant);
  // getting ExternalProperty from variants and checking them
  auto external_from_qt = qt_external_variant.value<ExternalProperty>();
  auto external_from_mvvm = std::get<ExternalProperty>(mvvm_external_variant);
  EXPECT_EQ(external_from_qt, external);
  EXPECT_EQ(external_from_mvvm, external);
}

//! Special test for comparison of Qt variants based on vector<double>.
//! We are not comparing those in normal application.

TEST_F(VariantConverterTest, QVariantComparisonForDoubleVector)
{
  const std::vector<double> vec({1.0, 2.0});
  QVariant variant1 = QVariant::fromValue(vec);
  QVariant variant2 = QVariant::fromValue(vec);

  // Direct variant comparison requires comparator registration.
  QMetaType::registerComparators<std::vector<double>>();
  EXPECT_EQ(variant1, variant2);
}

//! Special test for comparison of Qt variants based on ComboProperty.
//! We are not comparing those in normal application.

TEST_F(VariantConverterTest, QVariantComparisonForComboProperty)
{
  const ComboProperty combo = ComboProperty::CreateFrom({"a1", "a2"});
  QVariant variant1 = QVariant::fromValue(combo);
  QVariant variant2 = QVariant::fromValue(combo);

  // Direct variant comparison requires comparator registration.
  QMetaType::registerComparators<ComboProperty>();
  EXPECT_EQ(variant1, variant2);
}
