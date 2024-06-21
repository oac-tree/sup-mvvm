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

//! @file
//! Test for custom viewmodels to reproduce layouts which we often have in other GUIs.

#include "mvvm/viewmodel/viewmodel.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/abstract_row_strategy.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodel/viewmodel_controller_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <gtest/gtest.h>

#include <QTreeView>

namespace
{

/**
 * @brief The TableRowStrategy class is a custom strategy which forms a row with display name and
 * x,y,z values of vector item.
 */
class TableRowStrategy : public mvvm::AbstractRowStrategy
{
public:
  QStringList GetHorizontalHeaderLabels() const override
  {
    static const QStringList result = {"Name", "X", "Y", "Z"};
    return result;
  }

private:
  std::vector<std::unique_ptr<mvvm::ViewItem>> ConstructRowImpl(mvvm::SessionItem *item) override
  {
    std::vector<std::unique_ptr<mvvm::ViewItem>> result;

    if (auto vector = dynamic_cast<mvvm::VectorItem *>(item); vector)
    {
      result.emplace_back(mvvm::CreateDisplayNameViewItem(vector));
      result.emplace_back(mvvm::CreateDataViewItem(vector->GetItem(mvvm::VectorItem::kX)));
      result.emplace_back(mvvm::CreateDataViewItem(vector->GetItem(mvvm::VectorItem::kY)));
      result.emplace_back(mvvm::CreateDataViewItem(vector->GetItem(mvvm::VectorItem::kZ)));
    }

    return result;
  }
};

/**
 * @brief The TableViewModel class
 */
class TableViewModel : public mvvm::ViewModel
{
public:
  TableViewModel(mvvm::ISessionModel *model, QObject *parent = nullptr) : ViewModel(parent)
  {
    SetController(
        mvvm::factory::CreateController<mvvm::AllChildrenStrategy, TableRowStrategy>(model, this));
  }
};

}  // namespace

using namespace mvvm;

//! Testing custom viewmodels. This is an integration test to reproduce viewmodels similar to what
//! we have in other GUIs.

class CustomViewModelTest : public ::testing::Test
{
public:
};

TEST_F(CustomViewModelTest, InsertItem)
{
  ApplicationModel model;
  TableViewModel viewmodel(&model);

  model.InsertItem<VectorItem>();
  EXPECT_EQ(viewmodel.rowCount(), 1);
  EXPECT_EQ(viewmodel.columnCount(), 4);
}

TEST_F(CustomViewModelTest, StressInsertRemove)
{
  const int n_max_count{10};
  const int container_count{10};

  ApplicationModel model;
  TableViewModel viewmodel(&model);
  QTreeView view;
  view.setModel(&viewmodel);

  auto container0 = model.InsertItem<mvvm::ContainerItem>();

  for (int i = 0; i < n_max_count; ++i)
  {
    auto container1 = model.InsertItem<mvvm::ContainerItem>(container0);
    viewmodel.SetRootSessionItem(container1);

    // inserting
    for (int i_item = 0; i_item < container_count; ++i_item)
    {
      auto container2 = model.InsertItem<mvvm::ContainerItem>(container1);
      model.InsertItem<VectorItem>(container2);
      model.InsertItem<VectorItem>(container2);
      model.InsertItem<VectorItem>(container2);
    }

    model.RemoveItem(container1);
  }

  EXPECT_EQ(viewmodel.rowCount(), 0);
  EXPECT_EQ(viewmodel.columnCount(), 4);
}
