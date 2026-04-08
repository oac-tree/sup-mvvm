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

#include "mvvm/viewmodel/viewmodel.h"

#include <mvvm/model/application_model.h>
#include <mvvm/providers/abstract_row_strategy.h>
#include <mvvm/providers/standard_children_strategies.h>
#include <mvvm/providers/viewitem.h>
#include <mvvm/providers/viewitem_factory.h>
#include <mvvm/providers/viewmodel_controller_factory.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include <QSignalSpy>

namespace mvvm::test
{

namespace
{

constexpr auto kFixedLabel = "Fixed label";

/**
 * @brief The TestRowStrategy constructs a row of two cells: editable display name and static
 * label.
 */
class TestRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  std::size_t GetSize() const override { return 2U; }

  QStringList GetHorizontalHeaderLabels() const override
  {
    static const QStringList result = {"Name", "Label"};
    return result;
  }

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem* item) override
  {
    std::vector<std::unique_ptr<mvvm::ViewItem>> result;

    result.emplace_back(mvvm::CreateDisplayNameViewItem(item));
    result.emplace_back(mvvm::CreateLabelViewItem(item, kFixedLabel));

    return result;
  }
};

/**
 * @brief The TestViewModel class make use if TestRowStrategy.
 */
class TestViewModel : public mvvm::ViewModel
{
public:
  TestViewModel(mvvm::ISessionModel* model, QObject* parent = nullptr) : ViewModel(parent)
  {
    SetController(
        mvvm::factory::CreateController<mvvm::AllChildrenStrategy, TestRowStrategy>(model, this));
  }
};

}  // namespace

/**
 * @brief Tests for view model with custom label.
 *
 * It is based on real life bug, when changing display name of SessionItem caused data changed
 * signal for the label as well, which is not expected, because label is fixed and has no connection
 * with display name.
 */
class CustomLabelViewModelTest : public ::testing::Test
{
};

//! Changing editable display name of item, and expecting notifications only for display name
//! column, but not for label column. Real life bug.
TEST_F(CustomLabelViewModelTest, LabelPresentationItem)
{
  ApplicationModel model;
  auto item = model.InsertItem<PropertyItem>();
  item->SetData(42.0);
  item->SetFlag(mvvm::Appearance::kEditableDisplayName, true);

  TestViewModel viewmodel(&model);
  EXPECT_EQ(viewmodel.rowCount(QModelIndex()), 1);
  EXPECT_EQ(viewmodel.columnCount(QModelIndex()), 2);

  auto display_name_index = viewmodel.index(0, 0, QModelIndex());
  auto label_index = viewmodel.index(0, 1, QModelIndex());

  EXPECT_EQ(viewmodel.data(display_name_index, Qt::DisplayRole).toString(), "PropertyItem");
  EXPECT_EQ(viewmodel.data(label_index, Qt::DisplayRole).toString(), kFixedLabel);

  // changing of siplay name should cause only notifications related to column 0
  QSignalSpy spy_data_changed(&viewmodel, &ViewModelBase::dataChanged);

  item->SetDisplayName("New name");

  ASSERT_EQ(spy_data_changed.count(), 1);
  const QList<QVariant> arguments = spy_data_changed.takeFirst();
  EXPECT_EQ(arguments.size(), 3);  // QModelIndex left, QModelIndex right, QVector<int> roles
  EXPECT_EQ(arguments.at(0).value<QModelIndex>(), display_name_index);
  EXPECT_EQ(arguments.at(1).value<QModelIndex>(), display_name_index);
  const QVector<int> expected_roles = {Qt::DisplayRole, Qt::EditRole};
  EXPECT_EQ(arguments.at(2).value<QVector<int>>(), expected_roles);
}

}  // namespace mvvm::test
