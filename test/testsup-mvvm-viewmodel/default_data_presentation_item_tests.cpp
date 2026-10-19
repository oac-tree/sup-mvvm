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

#include "mvvm/providers/default_data_presentation_item.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/providers/custom_variants.h>
#include <mvvm/standarditems/editor_constants.h>

#include <gtest/gtest.h>

#include <QColor>

namespace mvvm::test
{

/**
 * @brief Tests for DefaultDataPresentationItem class.
 */
class DefaultDataPresentationItemTest : public ::testing::Test
{
};

TEST_F(DefaultDataPresentationItemTest, NullItem)
{
  const DefaultDataPresentationItem presentation(nullptr, 0);
  EXPECT_EQ(presentation.GetMainDataRole(), 0);
  EXPECT_FALSE(presentation.IsEnabled());
  EXPECT_FALSE(presentation.IsEditable());
  EXPECT_FALSE(presentation.IsCheckable());

  EXPECT_FALSE(presentation.Data(Qt::DisplayRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::ToolTipRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::DecorationRole).isValid());

  EXPECT_FALSE(presentation.GetDisplayRole().isValid());
}

// ------------------------------------------------------------------------------------------------
// Item carries an integer
// ------------------------------------------------------------------------------------------------

TEST_F(DefaultDataPresentationItemTest, ItemWithIntAndDataRoles)
{
  const auto main_data_role = DataRole::kData;

  SessionItem item;
  item.SetData(42);

  const DefaultDataPresentationItem presentation(&item, main_data_role);
  EXPECT_EQ(presentation.GetMainDataRole(), main_data_role);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());
  EXPECT_FALSE(presentation.IsCheckable());

  // data for various roles
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("42"));
  EXPECT_TRUE(utils::IsIntVariant(presentation.Data(Qt::EditRole)));
  EXPECT_EQ(presentation.Data(Qt::EditRole).toInt(), 42);
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::ToolTipRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::DecorationRole).isValid());

  // adjusting item text color
  item.SetData("#ff0000", DataRole::kTextColor);
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole).value<QColor>(), QColor(Qt::red));

  // modifying item check state
  item.SetData(Qt::Checked, DataRole::kCheckState);
  EXPECT_TRUE(presentation.IsCheckable());
  EXPECT_TRUE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
}

TEST_F(DefaultDataPresentationItemTest, ItemWithIntAndDataRolesToEmit)
{
  const auto main_data_role = DataRole::kData;

  SessionItem item;
  item.SetData(42);

  const DefaultDataPresentationItem presentation(&item, main_data_role);

  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kData),
            QVector<int>({Qt::DisplayRole, Qt::EditRole}));

  // DataRole::kDisplay (item display name) doesn't have Qt equivalent, so no roles to emit
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDisplay).isEmpty());

  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kAppearance),
            QVector<int>({Qt::ForegroundRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kTooltip), QVector<int>({Qt::ToolTipRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kTextColor),
            QVector<int>({Qt::ForegroundRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kCheckState),
            QVector<int>({Qt::CheckStateRole}));

  // other roles doesn't have Qt equivalent
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kIdentifier).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kEditor).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kLowerLimit).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kUpperLimit).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDataBackup).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kSelected).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kUser).isEmpty());
}

TEST_F(DefaultDataPresentationItemTest, ItemWithIntAndSetData)
{
  const auto main_data_role = DataRole::kData;

  SessionItem item;
  item.SetData(42);
  DefaultDataPresentationItem presentation(&item, main_data_role);

  const int new_data = 43;

  EXPECT_TRUE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("43"));    // new data
  EXPECT_EQ(presentation.Data(Qt::EditRole), QVariant::fromValue(new_data));  // new data

  // SessionItem itself should have new data
  EXPECT_EQ(item.Data<mvvm::int32>(), new_data);  // new data

  // attempt to set same data
  EXPECT_FALSE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));

  // changing check state
  EXPECT_TRUE(presentation.SetData(Qt::Checked, Qt::CheckStateRole));
  EXPECT_TRUE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
  EXPECT_EQ(item.Data<mvvm::int32>(DataRole::kCheckState), Qt::Checked);

  // it is not possible to change other Qt roles
  EXPECT_FALSE(presentation.SetData(QString("display"), Qt::DisplayRole));
  EXPECT_FALSE(presentation.SetData(QString("tooltip"), Qt::ToolTipRole));

  // it is not allowed to set another type of data for EditRole
  const QVariant not_allowed_value("Layer");
  EXPECT_THROW(presentation.SetData(not_allowed_value, Qt::EditRole), RuntimeException);
}

// ------------------------------------------------------------------------------------------------
// Item has no data for main data role
// ------------------------------------------------------------------------------------------------

TEST_F(DefaultDataPresentationItemTest, ItemWithoutDataAndDataRoles)
{
  const auto main_data_role = DataRole::kData;

  SessionItem item;
  const DefaultDataPresentationItem presentation(&item, main_data_role);
  EXPECT_EQ(presentation.GetMainDataRole(), main_data_role);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());
  EXPECT_FALSE(presentation.IsCheckable());

  EXPECT_FALSE(presentation.Data(Qt::DisplayRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString());

  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::ToolTipRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::DecorationRole).isValid());

  EXPECT_FALSE(presentation.GetDisplayRole().isValid());

  // it is still possible to adjust item's text color role and check state
  item.SetData("#ff0000", DataRole::kTextColor);
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole).value<QColor>(), QColor(Qt::red));
  item.SetData(Qt::Checked, DataRole::kCheckState);
  EXPECT_TRUE(presentation.Data(Qt::CheckStateRole).isValid());
  EXPECT_EQ(presentation.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
}

TEST_F(DefaultDataPresentationItemTest, ItemWithoutDataAndDataRolesToEmit)
{
  const auto main_data_role = DataRole::kData;

  SessionItem item;

  const DefaultDataPresentationItem presentation(&item, main_data_role);

  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kData),
            QVector<int>({Qt::DisplayRole, Qt::EditRole}));
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDisplay).isEmpty());

  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kAppearance),
            QVector<int>({Qt::ForegroundRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kTooltip), QVector<int>({Qt::ToolTipRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kTextColor),
            QVector<int>({Qt::ForegroundRole}));
  EXPECT_EQ(presentation.GetQtRolesToEmit(DataRole::kCheckState),
            QVector<int>({Qt::CheckStateRole}));

  // other roles doesn't have Qt equivalent
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kIdentifier).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kEditor).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kLowerLimit).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kUpperLimit).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kDataBackup).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kSelected).isEmpty());
  EXPECT_TRUE(presentation.GetQtRolesToEmit(DataRole::kUser).isEmpty());
}

TEST_F(DefaultDataPresentationItemTest, ItemWithoutDataAndSetData)
{
  const auto main_data_role = DataRole::kData;

  SessionItem item;

  DefaultDataPresentationItem presentation(&item, main_data_role);

  // original item doesn't have a data, but currenlty it is possible to set any data as EditRole
  const int new_data = 43;

  EXPECT_TRUE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("43"));    // new data
  EXPECT_EQ(presentation.Data(Qt::EditRole), QVariant::fromValue(new_data));  // new data

  // SessionItem itself should have new data
  EXPECT_EQ(item.Data<mvvm::int32>(), new_data);  // new data
}

// ------------------------------------------------------------------------------------------------
// Mixture of old use cases
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// Nullable item
// ------------------------------------------------------------------------------------------------

TEST_F(DefaultDataPresentationItemTest, NullableItemWithoutData)
{
  SessionItem item;
  item.SetFlag(Appearance::kNullable, true);
  item.SetData(uint32{42}, DataRole::kDefaultValue);

  const DefaultDataPresentationItem presentation(&item, DataRole::kData);

  // placeholder text in gray, no data to edit
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("default"));
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole).value<QColor>(), QColor(Qt::gray));
  EXPECT_FALSE(presentation.Data(Qt::EditRole).isValid());

  // own placeholder text
  item.SetData(std::string("required"), DataRole::kPlaceholder);
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("required"));
}

TEST_F(DefaultDataPresentationItemTest, NullableItemWithData)
{
  SessionItem item;
  item.SetFlag(Appearance::kNullable, true);
  item.SetData(uint32{42}, DataRole::kDefaultValue);
  item.SetData(uint32{10});

  DefaultDataPresentationItem presentation(&item, DataRole::kData);

  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("10"));
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  // invalid variant removes the data, placeholder is shown
  EXPECT_TRUE(presentation.SetData(QVariant(), Qt::EditRole));
  EXPECT_FALSE(item.HasData(DataRole::kData));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole).toString(), QString("default"));
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole).value<QColor>(), QColor(Qt::gray));
}

TEST_F(DefaultDataPresentationItemTest, NotNullableItemWithoutData)
{
  SessionItem item;
  item.SetData(std::string("required"), DataRole::kPlaceholder);

  const DefaultDataPresentationItem presentation(&item, DataRole::kData);

  // no placeholder without nullable flag
  EXPECT_FALSE(presentation.Data(Qt::DisplayRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());
}

TEST_F(DefaultDataPresentationItemTest, DataPresentationItemDataForDouble)
{
  const auto main_data_role = DataRole::kData;
  const double expected{42.0};

  SessionItem item;
  item.SetData(expected);

  DefaultDataPresentationItem presentation(&item, main_data_role);
  EXPECT_EQ(presentation.GetItem(), &item);
  EXPECT_EQ(presentation.GetMainDataRole(), DataRole::kData);
  EXPECT_TRUE(presentation.IsEnabled());
  EXPECT_TRUE(presentation.IsEditable());

  EXPECT_EQ(presentation.Data(Qt::EditRole), expected);
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QString("42.0"));
}

TEST_F(DefaultDataPresentationItemTest, DataPresentationItemSetDataForDouble)
{
  const auto main_data_role = DataRole::kData;
  SessionItem item;
  item.SetData(42.0);

  DefaultDataPresentationItem presentation(&item, main_data_role);

  const double new_data{43.0};

  // not possible to set DisplayRole
  EXPECT_FALSE(presentation.SetData(QVariant::fromValue(new_data), Qt::DisplayRole));

  EXPECT_TRUE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QString("43.0"));             // new data
  EXPECT_EQ(presentation.Data(Qt::EditRole), QVariant::fromValue(new_data));  // new data

  // SessionItem itself should have new data
  EXPECT_EQ(item.Data<double>(), new_data);  // new data

  // attempt to set same data twice
  EXPECT_FALSE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));

  // it is not allowed to set another type of data
  const QVariant not_allowed_value("Layer");
  EXPECT_THROW(presentation.SetData(not_allowed_value, Qt::EditRole), RuntimeException);
}

TEST_F(DefaultDataPresentationItemTest, DataPresentationItemForComboProperty)
{
  const auto main_data_role = DataRole::kData;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QMetaType::registerComparators<mvvm::ComboProperty>();
#endif

  const std::vector<std::string> expected_values({"abc", "def"});
  const ComboProperty property(expected_values);
  const QVariant expected_variant = QVariant::fromValue(property);

  SessionItem item;
  item.SetData(property);

  const DefaultDataPresentationItem presentation(&item, main_data_role);

  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QString("abc"));  // currently selected item
  EXPECT_EQ(presentation.Data(Qt::EditRole), expected_variant);
}

TEST_F(DefaultDataPresentationItemTest, DataPresentationItemForSecondaryDataRole)
{
  const int kSecondaryDataRole{42};
  SessionItem item;
  item.SetData(42.0);
  item.SetData(std::string("abc"), kSecondaryDataRole);

  DefaultDataPresentationItem presentation(&item, kSecondaryDataRole);

  const QString new_data("def");

  // not possible to set DisplayRole
  EXPECT_FALSE(presentation.SetData(QVariant::fromValue(new_data), Qt::DisplayRole));

  EXPECT_TRUE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));
  EXPECT_EQ(presentation.Data(Qt::DisplayRole), QVariant::fromValue(new_data));  // new data
  EXPECT_EQ(presentation.Data(Qt::EditRole), QVariant::fromValue(new_data));     // new data

  // SessionItem itself should have new data
  EXPECT_EQ(item.Data<std::string>(kSecondaryDataRole), new_data.toStdString());  // new data

  // attempt to set same data twice
  EXPECT_FALSE(presentation.SetData(QVariant::fromValue(new_data), Qt::EditRole));
}

TEST_F(DefaultDataPresentationItemTest, DataPresentationItemTooltipRole)
{
  const auto main_data_role = DataRole::kData;
  SessionItem item;

  const DefaultDataPresentationItem presentation(&item, main_data_role);
  EXPECT_FALSE(presentation.Data(Qt::ToolTipRole).isValid());

  item.SetToolTip("abc");
  EXPECT_EQ(presentation.Data(Qt::ToolTipRole).toString(), QString("abc"));
}

TEST_F(DefaultDataPresentationItemTest, DataPresentationItemForegroundRole)
{
  const auto main_data_role = DataRole::kData;
  SessionItem item;
  item.SetData(42);

  const DefaultDataPresentationItem presentation(&item, main_data_role);

  // Enabled item doesn't have valid foreground role.
  // This means that the color of text is left for a view to decide.
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  item.SetEnabled(false);
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole), QColor(Qt::gray));

  item.SetEnabled(true);
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  // Hidden item has normal color by default. It is up to the view to decide how to show "hidden"
  // item.
  item.SetVisible(false);
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());
}

TEST_F(DefaultDataPresentationItemTest, DataPresentationItemCheckStateRole)
{
  const auto main_data_role = DataRole::kData;

  // Item carrying integer doesn't have CheckStateRole
  SessionItem item1;
  item1.SetData(42);
  const DefaultDataPresentationItem presentation1(&item1, main_data_role);
  EXPECT_FALSE(presentation1.Data(Qt::CheckStateRole).isValid());

  // Item carrying bool has a CheckStateRole
  SessionItem item2;
  item2.SetData(false);
  const DefaultDataPresentationItem presentation2(&item2, main_data_role);
  EXPECT_TRUE(presentation2.Data(Qt::CheckStateRole).isValid());
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Unchecked);

  item2.SetData(true);
  EXPECT_EQ(presentation2.Data(Qt::CheckStateRole).toInt(), Qt::Checked);
}

//! Presentation-only Qt roles (background, foreground) are stored on board and are not propagated
//! to the underlying SessionItem.
TEST_F(DefaultDataPresentationItemTest, PresentationOnlyQtRoles)
{
  const auto main_data_role = DataRole::kData;
  SessionItem item;
  item.SetData(42);

  DefaultDataPresentationItem presentation(&item, main_data_role);

  // no presentation-only roles set initially
  EXPECT_FALSE(presentation.Data(Qt::BackgroundRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());
  EXPECT_FALSE(presentation.Data(Qt::DecorationRole).isValid());

  // setting background role
  EXPECT_TRUE(presentation.SetData(QColor(Qt::yellow), Qt::BackgroundRole));
  EXPECT_EQ(presentation.Data(Qt::BackgroundRole).value<QColor>(), QColor(Qt::yellow));

  // setting foreground role; it takes precedence over the item-derived value
  EXPECT_TRUE(presentation.SetData(QColor(Qt::red), Qt::ForegroundRole));
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole).value<QColor>(), QColor(Qt::red));

  // setting decoration role
  EXPECT_TRUE(presentation.SetData(QColor(Qt::blue), Qt::DecorationRole));
  EXPECT_EQ(presentation.Data(Qt::DecorationRole).value<QColor>(), QColor(Qt::blue));

  // presentation-only roles are not propagated to the SessionItem
  EXPECT_FALSE(item.HasData(DataRole::kTextColor));
  EXPECT_EQ(presentation.Data(Qt::EditRole).toInt(), 42);

  // an invalid variant resets the role to its default
  EXPECT_TRUE(presentation.SetData(QVariant(), Qt::BackgroundRole));
  EXPECT_FALSE(presentation.Data(Qt::BackgroundRole).isValid());

  EXPECT_TRUE(presentation.SetData(QVariant(), Qt::ForegroundRole));
  EXPECT_FALSE(presentation.Data(Qt::ForegroundRole).isValid());

  EXPECT_TRUE(presentation.SetData(QVariant(), Qt::DecorationRole));
  EXPECT_FALSE(presentation.Data(Qt::DecorationRole).isValid());
}

//! An explicitly set decoration role overrides the value derived from a color-editor item.
TEST_F(DefaultDataPresentationItemTest, DecorationRolePrecedenceOverColorEditor)
{
  const auto main_data_role = DataRole::kData;
  SessionItem item;
  EXPECT_TRUE(item.SetData("red"));
  item.SetEditorType(constants::kColorEditorType);

  DefaultDataPresentationItem presentation(&item, main_data_role);

  // item-derived decoration for a color editor
  EXPECT_EQ(presentation.Data(Qt::DecorationRole).value<QColor>(), QColor("red"));

  // explicit decoration wins over the item-derived value
  EXPECT_TRUE(presentation.SetData(QColor(Qt::blue), Qt::DecorationRole));
  EXPECT_EQ(presentation.Data(Qt::DecorationRole).value<QColor>(), QColor(Qt::blue));

  // resetting falls back to the item-derived value
  EXPECT_TRUE(presentation.SetData(QVariant(), Qt::DecorationRole));
  EXPECT_EQ(presentation.Data(Qt::DecorationRole).value<QColor>(), QColor("red"));
}

//! Only background, foreground and decoration roles are reported as presentation-only.
TEST_F(DefaultDataPresentationItemTest, IsPresentationOnlyRole)
{
  const auto main_data_role = DataRole::kData;
  SessionItem item;
  item.SetData(42);

  const DefaultDataPresentationItem presentation(&item, main_data_role);

  EXPECT_TRUE(presentation.IsPresentationOnlyRole(Qt::BackgroundRole));
  EXPECT_TRUE(presentation.IsPresentationOnlyRole(Qt::ForegroundRole));
  EXPECT_TRUE(presentation.IsPresentationOnlyRole(Qt::DecorationRole));

  EXPECT_FALSE(presentation.IsPresentationOnlyRole(Qt::DisplayRole));
  EXPECT_FALSE(presentation.IsPresentationOnlyRole(Qt::EditRole));
  EXPECT_FALSE(presentation.IsPresentationOnlyRole(Qt::CheckStateRole));
  EXPECT_FALSE(presentation.IsPresentationOnlyRole(Qt::ToolTipRole));
}

//! The explicitly set foreground role overrides the value derived from a disabled item.
TEST_F(DefaultDataPresentationItemTest, ForegroundRolePrecedenceOverDisabledItem)
{
  const auto main_data_role = DataRole::kData;
  SessionItem item;
  item.SetData(42);
  item.SetEnabled(false);

  DefaultDataPresentationItem presentation(&item, main_data_role);

  // disabled item provides gray foreground
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole).value<QColor>(), QColor(Qt::gray));

  // explicit foreground wins over the item-derived gray
  EXPECT_TRUE(presentation.SetData(QColor(Qt::red), Qt::ForegroundRole));
  EXPECT_EQ(presentation.Data(Qt::ForegroundRole).value<QColor>(), QColor(Qt::red));
}

TEST_F(DefaultDataPresentationItemTest, DataPresentationItemDecorationRoleForColorEditor)
{
  const auto main_data_role = DataRole::kData;

  // create SessionItem with data on board
  SessionItem item;
  EXPECT_TRUE(item.SetData("red"));
  item.SetEditorType(constants::kColorEditorType);

  const DefaultDataPresentationItem viewItem(&item, main_data_role);
  EXPECT_EQ(viewItem.Data(Qt::EditRole), QString("red"));
  EXPECT_EQ(viewItem.Data(Qt::DisplayRole), QString("red"));
  EXPECT_EQ(viewItem.Data(Qt::DecorationRole).value<QColor>(), QColor("red"));
}

}  // namespace mvvm::test
