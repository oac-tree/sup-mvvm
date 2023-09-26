/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/variant_converter.h"

#include <mvvm/core/exceptions.h>

#include <gtest/gtest.h>

#include <QColor>
#include <QDebug>

using namespace mvvm;

//! Tests for utilities related to variant conversion.

class VariantConverterTests : public ::testing::Test
{
};

//! Testing original method to construct QVariant from std::variant. Please note that method
//! QVariant::canConvert can converts in many close types.

TEST_F(VariantConverterTests, fromStdVariant)
{
  {
    auto variant = QVariant::fromStdVariant(variant_t{});
    EXPECT_FALSE(variant.isValid());
  }

  {
    auto variant = QVariant::fromStdVariant(variant_t{true});
    EXPECT_TRUE(utils::IsBoolVariant(variant));
    EXPECT_EQ(std::string(variant.typeName()), constants::kBooleanQtTypeName);
  }

  {
    const mvvm::char8 value{0};
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::char8>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kChar8QtTypeName);
  }

  {
    const mvvm::int8 value{0};
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::int8>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kInt8QtTypeName);
  }

  {
    const mvvm::uint8 value{0};
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::uint8>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kUInt8QtTypeName);
  }

  {
    const mvvm::int16 value{0};
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::int16>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kInt16QtTypeName);
  }

  {
    const mvvm::uint16 value{0};
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::uint16>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kUInt16QtTypeName);
  }

  {
    const mvvm::int32 value{0};
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::int32>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kInt32QtTypeName);
  }

  {
    const mvvm::uint32 value{0};
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::uint32>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kUInt32QtTypeName);
  }

  {
    const mvvm::int64 value{0};
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::int64>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kInt64QtTypeName);
  }

  {
    const mvvm::uint64 value{0};
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::uint64>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kUInt64QtTypeName);
  }

  {
    const mvvm::float32 value{0.0};
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::float32>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kFloat32QtTypeName);
  }

  {
    const mvvm::float64 value{0.0};
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::float64>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kFloat64QtTypeName);
  }

  {
    std::string value;
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<std::string>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kStdStringQtTypeName);
  }

  {
    std::vector<double> value;
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<std::vector<double>>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kStdVectorDoubleQtTypeName);
  }

  {
    ComboProperty value;
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<ComboProperty>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kComboPropertyQtTypeName);
  }

  {
    ExternalProperty value;
    auto variant = QVariant::fromStdVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<ExternalProperty>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kExternalPropertyQtTypeName);
  }
}

//! Testing function to convert Qt variant to std variant.

TEST_F(VariantConverterTests, GetStdVariant)
{
  EXPECT_EQ(GetStdVariant(QVariant()), variant_t());
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(true)), variant_t(true));

  auto std_variant_from_qint = GetStdVariant(QVariant::fromValue(42));
  EXPECT_EQ(utils::TypeName(std_variant_from_qint), constants::kInt32TypeName);
  EXPECT_EQ(std_variant_from_qint, variant_t(42));

  const qint64 num(42);
  auto std_variant_from_qint64 = GetStdVariant(QVariant(num));
  EXPECT_EQ(utils::TypeName(std_variant_from_qint64), constants::kInt64TypeName);
  EXPECT_EQ(std_variant_from_qint64, variant_t(42L));

  EXPECT_EQ(GetStdVariant(QVariant::fromValue(1.0)), variant_t(1.0));
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(QString("abc"))), variant_t(std::string("abc")));

  const std::vector<double> vec{1.0, 2.0};
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(vec)), variant_t(vec));

  const ComboProperty combo = ComboProperty::CreateFrom({"a1"});
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(combo)), variant_t(combo));

  const ExternalProperty external_property("text", "color", "identifier");
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(external_property)), variant_t(external_property));

  const RealLimits real_limits = RealLimits::CreateLimited(1.0, 2.0);
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(real_limits)), variant_t(real_limits));

  const IntLimits int_limits = IntLimits::CreateLimited(1, 42);
  EXPECT_EQ(GetStdVariant(QVariant::fromValue(int_limits)), variant_t(int_limits));

  // other Qt variants are unsupported
  EXPECT_THROW(GetStdVariant(QVariant(QColor(Qt::red))), RuntimeException);
}

//! Testing function to convert std variant to Qt variant.

TEST_F(VariantConverterTests, GetQtVariant)
{
  // from undefined variant
  EXPECT_EQ(GetQtVariant(variant_t()), QVariant());

  // from bool
  EXPECT_EQ(GetQtVariant(variant_t(true)), QVariant::fromValue(true));
  EXPECT_EQ(GetQtVariant(variant_t(false)), QVariant::fromValue(false));

  // from int
  EXPECT_EQ(GetQtVariant(variant_t(42)), QVariant::fromValue(42));
  EXPECT_EQ(QString(GetQtVariant(variant_t(42)).typeName()), QString("int"));
  EXPECT_EQ(GetQtVariant(variant_t(42)).type(), QMetaType::Int);

  // from int64 (the same)
  EXPECT_EQ(GetQtVariant(variant_t(42L)), QVariant::fromValue(42L));
  EXPECT_EQ(QString(GetQtVariant(variant_t(42L)).typeName()), QString("qlonglong"));
  EXPECT_EQ(GetQtVariant(variant_t(42L)).type(), QMetaType::LongLong);

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

  // from RealLimits
  const RealLimits limits = RealLimits::CreateLimited(1.0, 2.0);
  variant_t limits_variant(limits);
  auto qt_limits_variant = GetQtVariant(limits_variant);
  EXPECT_EQ(qt_limits_variant.value<RealLimits>(), limits);
}

//! Special test for comparison of Qt variants based on vector<double>.
//! We are not comparing those in normal application.

TEST_F(VariantConverterTests, QVariantComparisonForDoubleVector)
{
  const std::vector<double> vec({1.0, 2.0});
  QVariant variant1 = QVariant::fromValue(vec);
  QVariant variant2 = QVariant::fromValue(vec);

  // Direct variant comparison requires comparator registration.

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QMetaType::registerComparators<std::vector<double>>();
#endif
  EXPECT_EQ(variant1, variant2);
}

//! Special test for comparison of Qt variants based on ComboProperty.
//! We are not comparing those in normal application.

TEST_F(VariantConverterTests, QVariantComparisonForComboProperty)
{
  const ComboProperty combo = ComboProperty::CreateFrom({"a1", "a2"});
  QVariant variant1 = QVariant::fromValue(combo);
  QVariant variant2 = QVariant::fromValue(combo);

  // Direct variant comparison requires comparator registration.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QMetaType::registerComparators<ComboProperty>();
#endif
  EXPECT_EQ(variant1, variant2);
}
