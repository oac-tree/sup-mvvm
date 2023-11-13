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

class VariantConverterTest : public ::testing::Test
{
};

//! Testing original method to construct QVariant from std::variant.

TEST_F(VariantConverterTest, fromStdVariant)
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

//! Testing function to convert Qt variant to std variant. Please note, that QVariant has limited
//! set of constructors, QVariant::fromValue shall be used.

TEST_F(VariantConverterTest, GetStdVariant)
{
  {
    EXPECT_EQ(GetStdVariant(QVariant()), variant_t());
    EXPECT_EQ(GetStdVariant(QVariant::fromValue(true)), variant_t(true));
  }

  {
    auto variant = GetStdVariant(QVariant{true});
    EXPECT_EQ(GetTypeCode(variant), TypeCode::Bool);
  }

  {
    const mvvm::char8 value{0};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::Char8);
  }

  {
    const mvvm::int8 value{0};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::Int8);
  }

  {
    const mvvm::uint8 value{0};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::UInt8);
  }

  {
    const mvvm::int16 value{0};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::Int16);
  }

  {
    const mvvm::uint16 value{0};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::UInt16);
  }

  {
    const mvvm::int32 value{42};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::Int32);
    EXPECT_EQ(variant, variant_t(value));
  }

  {
    const mvvm::uint32 value{42};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::UInt32);
    EXPECT_EQ(variant, variant_t(value));
  }

  {
    const mvvm::int64 value{42};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::Int64);
    EXPECT_EQ(variant, variant_t(value));
  }

  {
    const mvvm::uint64 value{42};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::UInt64);
    EXPECT_EQ(variant, variant_t(value));
  }

  // This test is failing on CODAC. It sees mvvm::int64 and qint64 as different types.
  // FIXME do we need separate support to qint64? Consider removing.

  //  {
  //    const qint64 value{42};
  //    auto qt_variant = QVariant::fromValue(value);
  //    auto variant = GetStdVariant(qt_variant);
  //    EXPECT_EQ(GetTypeCode(variant), TypeCode::Int64);
  //    EXPECT_EQ(variant, variant_t(value));
  //  }

  {
    const mvvm::float32 value{42.1};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::Float32);
    EXPECT_EQ(variant, variant_t(value));
  }

  {
    const QString value{"abc"};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::String);
    EXPECT_EQ(variant, variant_t(value.toStdString()));
  }

  {
    const std::string value{"abc"};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::String);
    EXPECT_EQ(variant, variant_t(value));
  }

  {
    const std::vector<double> value{1.0, 2.0};
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::VectorOfDouble);
    EXPECT_EQ(variant, variant_t(value));
  }

  {
    const ComboProperty value = ComboProperty::CreateFrom({"a1"});
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::ComboProperty);
    EXPECT_EQ(variant, variant_t(value));
  }

  {
    const ExternalProperty value("text", "color", "identifier");
    auto qt_variant = QVariant::fromValue(value);
    auto variant = GetStdVariant(qt_variant);
    EXPECT_EQ(GetTypeCode(variant), TypeCode::ExternalProperty);
    EXPECT_EQ(variant, variant_t(value));
  }

  // other Qt variants are unsupported
  EXPECT_THROW(GetStdVariant(QVariant(QColor(Qt::red))), RuntimeException);
}

//! Testing method GetQtVariant. It is slightly different from original QVariant::fromStdVariant.

TEST_F(VariantConverterTest, GetQtVariant)
{
  {
    auto variant = GetQtVariant(variant_t{});
    EXPECT_FALSE(variant.isValid());
  }

  {
    auto variant = GetQtVariant(variant_t{true});
    EXPECT_TRUE(utils::IsBoolVariant(variant));
    EXPECT_EQ(std::string(variant.typeName()), constants::kBooleanQtTypeName);
  }

  {
    const mvvm::char8 value{0};
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::char8>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kChar8QtTypeName);
  }

  {
    const mvvm::int8 value{0};
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::int8>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kInt8QtTypeName);
  }

  {
    const mvvm::uint8 value{0};
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::uint8>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kUInt8QtTypeName);
  }

  {
    const mvvm::int16 value{0};
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::int16>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kInt16QtTypeName);
  }

  {
    const mvvm::uint16 value{0};
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::uint16>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kUInt16QtTypeName);
  }

  {
    const mvvm::int32 value{0};
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::int32>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kInt32QtTypeName);
  }

  {
    const mvvm::uint32 value{0};
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::uint32>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kUInt32QtTypeName);
  }

  {
    const mvvm::int64 value{0};
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::int64>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kInt64QtTypeName);
  }

  {
    const mvvm::uint64 value{0};
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::uint64>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kUInt64QtTypeName);
  }

  {
    const mvvm::float32 value{0.0};
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::float32>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kFloat32QtTypeName);
  }

  {
    const mvvm::float64 value{0.0};
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<mvvm::float64>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kFloat64QtTypeName);
  }

  {
    std::string value;
    auto variant = GetQtVariant(variant_t{value});
    // different with the original QVariant::GetQtVariant
    //    EXPECT_TRUE(variant.canConvert<std::string>());
    //    EXPECT_EQ(std::string(variant.typeName()), constants::kStdStringQtTypeName);
    EXPECT_TRUE(variant.canConvert<QString>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kStringQtTypeName);
  }

  {
    std::vector<double> value;
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<std::vector<double>>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kStdVectorDoubleQtTypeName);
  }

  {
    ComboProperty value;
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<ComboProperty>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kComboPropertyQtTypeName);
  }

  {
    ExternalProperty value;
    auto variant = GetQtVariant(variant_t{value});
    EXPECT_TRUE(variant.canConvert<ExternalProperty>());
    EXPECT_EQ(std::string(variant.typeName()), constants::kExternalPropertyQtTypeName);
  }
}

//! Testing function to convert std variant to Qt variant.

TEST_F(VariantConverterTest, GetQtVariantExtended)
{
  // from undefined variant
  EXPECT_EQ(GetQtVariant(variant_t()), QVariant());

  // from bool
  EXPECT_EQ(GetQtVariant(variant_t(true)), QVariant::fromValue(true));
  EXPECT_EQ(GetQtVariant(variant_t(false)), QVariant::fromValue(false));

  // from int
  EXPECT_EQ(GetQtVariant(variant_t(42)), QVariant::fromValue(42));
  EXPECT_EQ(utils::GetQtVariantName(GetQtVariant(variant_t(42))), constants::kInt32QtTypeName);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  EXPECT_EQ(GetQtVariant(variant_t(42)).type(), QMetaType::Int);
#else
  EXPECT_EQ(GetQtVariant(variant_t(42)).typeId(), QMetaType::Int);
#endif

  // from int64
  EXPECT_EQ(GetQtVariant(variant_t(42L)), QVariant::fromValue(42L));
  EXPECT_EQ(utils::GetQtVariantName(GetQtVariant(variant_t(42L))), constants::kInt64QtTypeName);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  EXPECT_EQ(GetQtVariant(variant_t(42L)).type(), QMetaType::Long);
#else
  EXPECT_EQ(GetQtVariant(variant_t(42L)).typeId(), QMetaType::Long);
#endif

  // from double
  EXPECT_EQ(GetQtVariant(variant_t(1.1)), QVariant::fromValue(1.1));

  // from std::string to QString
  const std::string str{"abc"};
  const variant_t mvvm_str_variant(str);
  const QVariant qt_str_variant(QString::fromStdString(str));
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
  const QVariant variant1 = QVariant::fromValue(vec);
  const QVariant variant2 = QVariant::fromValue(vec);

  // Direct variant comparison requires comparator registration.

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QMetaType::registerComparators<std::vector<double>>();
#endif
  EXPECT_EQ(variant1, variant2);
}

//! Special test for comparison of Qt variants based on ComboProperty.
//! We are not comparing those in normal application.

TEST_F(VariantConverterTest, QVariantComparisonForComboProperty)
{
  const ComboProperty combo = ComboProperty::CreateFrom({"a1", "a2"});
  const QVariant variant1 = QVariant::fromValue(combo);
  const QVariant variant2 = QVariant::fromValue(combo);

  // Direct variant comparison requires comparator registration.
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QMetaType::registerComparators<ComboProperty>();
#endif
  EXPECT_EQ(variant1, variant2);
}
