/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Daniel Chichorro (EXT)
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
#include "mvvm/viewmodel/all_items_viewmodel.h"
#include "mvvm/viewmodel/viewmodel_controller.h"
#include "mvvm/viewmodel/viewmodel_controller_impl.h"

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/interfaces/children_strategy_interface.h>
#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/standarditems/vector_item.h>
#include <mvvm/viewmodel/standard_children_strategies.h>
#include <mvvm/viewmodel/standard_row_strategies.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>
#include <mvvm/viewmodelbase/viewmodel_base.h>

#include <gtest/gtest.h>
#include <testutils/mock_event_listener.h>

#include <stack>

using namespace mvvm;

// Children strategy that excludes container items from the model but shows their children
// as if they were direct children of the container's parent. All other items are shown as is.
class TestExcludeContainerStrategy : public ChildrenStrategyInterface
{
public:
  std::vector<SessionItem*> GetChildren(const SessionItem* item) const
  {
    std::vector<SessionItem*> result{};

    if (item == nullptr || dynamic_cast<const ContainerItem*>(item) != nullptr)
    {
      return result;
    }

    std::stack<SessionItem*> stack{};
    auto top_children = item->GetAllItems();
    for (auto it = top_children.rbegin(); it != top_children.rend(); ++it)
    {
      stack.push(*it);
    }

    while (!stack.empty())
    {
      const auto top_item = stack.top();
      stack.pop();

      if (top_item->IsVisible())
      {
        if (dynamic_cast<ContainerItem*>(top_item) != nullptr)
        {
          // found a container, push its children to the stack and skip it
          auto container_children = top_item->GetAllItems();
          for (auto it = container_children.rbegin(); it != container_children.rend(); ++it)
          {
            stack.push(*it);
          }
        }
        else
        {
          // push all children that are not containers to the result
          result.push_back(top_item);
        }
      }
    }

    return result;
  }
};

// View model with TestExcludeContainerStrategy as children strategy
class TestViewModel : public ViewModel
{
public:
  explicit TestViewModel(SessionModelInterface* model, QObject* parent = nullptr)
      : ViewModel(parent)
  {
    SetController(
        factory::CreateController<TestExcludeContainerStrategy, LabelDataRowStrategy>(model, this));
  };
};

// Test item that can be used as a root item in the model
class TestItem : public CompoundItem
{
public:
  static inline const std::string Type = "TestItem";
  static inline const std::string kChildren = "kChildren";

  explicit TestItem(const std::string& model_type = Type) : CompoundItem(model_type)
  {
    RegisterTag(TagInfo::CreateUniversalTag(kChildren), /*set_as_default*/ true);
  };
};

//! Tests for ViewModelControler Virtual parentage handling.
class VirtualParentTests : public ::testing::Test
{
public:
  void SetUp() override
  {
    m_model = std::make_unique<ApplicationModel>();
  }

protected:
  std::unique_ptr<SessionModelInterface> m_model;
};

// Controls test to verify the default behaviour, the model topology is as follows:
//
//  ┌──────────┐
//  │Root item │
//  └────┬─────┘
//       │
//       ▼
//  ┌──────────┐
//  │Container0│
//  └────┬─────┘
//       ├─────────────┬────────────┐
//       ▼             ▼            ▼
//  ┌──────────┐  ┌─────────┐  ┌─────────┐
//  │Container1│  │Property0│  │Property1│
//  └────┬─────┘  └─────────┘  └─────────┘
//       ├─────────────┐
//       ▼             ▼
//  ┌──────────┐  ┌──────────┐
//  │Property2 │  │Property3 │
//  └──────────┘  └──────────┘
//
TEST_F(VirtualParentTests, Control)
{
  const auto root_item = m_model->InsertItem<TestItem>();

  const auto container0 = m_model->InsertItem<ContainerItem>(root_item);
  const auto container1 = m_model->InsertItem<ContainerItem>(container0);

  const auto property0 = m_model->InsertItem<PropertyItem>(container0);
  const auto property1 = m_model->InsertItem<PropertyItem>(container0);
  const auto property2 = m_model->InsertItem<PropertyItem>(container1);
  const auto property3 = m_model->InsertItem<PropertyItem>(container1);

  AllItemsViewModel viewmodel(m_model.get());

  // the model contains only one root entry (root item)
  const auto root_index = viewmodel.indexFromItem(viewmodel.rootItem());
  EXPECT_EQ(viewmodel.rowCount(), 1);

  // root item index
  const auto root_item_index = viewmodel.index(0, 0, root_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(root_item_index), root_item);

  // the root item has 1 children (container0)
  EXPECT_EQ(viewmodel.rowCount(root_item_index), 1);

  const auto container0_index = viewmodel.index(0, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(container0_index), container0);

  // container0 has 3 children (container1, property0, property1)
  EXPECT_EQ(viewmodel.rowCount(container0_index), 3);

  const auto container1_index = viewmodel.index(0, 0, container0_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(container1_index), container1);

  const auto property0_index = viewmodel.index(1, 0, container0_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property0_index), property0);

  const auto property1_index = viewmodel.index(2, 0, container0_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property1_index), property1);

  // container1 has 2 children (property2, property3)
  EXPECT_EQ(viewmodel.rowCount(container1_index), 2);

  const auto property2_index = viewmodel.index(0, 0, container1_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property2_index), property2);

  const auto property3_index = viewmodel.index(1, 0, container1_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property3_index), property3);
}

// Test the initial state for a model with virtual parent items, the model topology is shared
// with the control test above. The resulting representation should be as follows:
//
//  ┌──────────┐
//  │Root item │
//  └────┬─────┘
//       ├─────────────┬────────────┬────────────┐
//       ▼             ▼            ▼            ▼
//  ┌──────────┐  ┌──────────┐  ┌─────────┐  ┌─────────┐
//  │Property2 │  │Property3 │  │Property0│  │Property1│
//  └──────────┘  └──────────┘  └─────────┘  └─────────┘
//
TEST_F(VirtualParentTests, InitialState)
{
  const auto root_item = m_model->InsertItem<TestItem>();

  const auto container0 = m_model->InsertItem<ContainerItem>(root_item);
  const auto container1 = m_model->InsertItem<ContainerItem>(container0);

  const auto property0 = m_model->InsertItem<PropertyItem>(container0);
  const auto property1 = m_model->InsertItem<PropertyItem>(container0);
  const auto property2 = m_model->InsertItem<PropertyItem>(container1);
  const auto property3 = m_model->InsertItem<PropertyItem>(container1);

  TestViewModel viewmodel(m_model.get());

  // the model contains only one root entry (root item)
  const auto root_index = viewmodel.indexFromItem(viewmodel.rootItem());
  EXPECT_EQ(viewmodel.rowCount(), 1);

  // root item index
  const auto root_item_index = viewmodel.index(0, 0, root_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(root_item_index), root_item);

  // the root item has 4 children (property0, property1, property2, property3)
  EXPECT_EQ(viewmodel.rowCount(root_item_index), 4);

  // properties 2 and 3 are first as they are children of container1 which is inserted first
  const auto property2_index = viewmodel.index(0, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property2_index), property2);

  const auto property3_index = viewmodel.index(1, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property3_index), property3);

  // properties 0 and 1 are next as they are inserted after container1
  const auto property0_index = viewmodel.index(2, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property0_index), property0);

  const auto property1_index = viewmodel.index(3, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property1_index), property1);

  // container0 and container1 are not in the model
  EXPECT_EQ(viewmodel.FindViews(container0).size(), 0);
  EXPECT_EQ(viewmodel.FindViews(container1).size(), 0);
}

TEST_F(VirtualParentTests, ContainerAsRootItem)
{
  const auto root_container = m_model->InsertItem<ContainerItem>();
  const auto property0 = m_model->InsertItem<PropertyItem>(root_container);
  const auto property1 = m_model->InsertItem<PropertyItem>(root_container);

  TestViewModel viewmodel(m_model.get());

  // the model contains 2 children (property0, property1)
  const auto root_index = viewmodel.indexFromItem(viewmodel.rootItem());
  EXPECT_EQ(viewmodel.rowCount(), 2);

  const auto property0_index = viewmodel.index(0, 0, root_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property0_index), property0);

  const auto property1_index = viewmodel.index(1, 0, root_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property1_index), property1);

  // container is not in the model
  EXPECT_EQ(viewmodel.FindViews(root_container).size(), 0);
}

TEST_F(VirtualParentTests, InsertVirtualParent)
{
  const auto root_item = m_model->InsertItem<TestItem>();

  auto container_item = std::make_unique<ContainerItem>();

  const auto container = container_item.get();
  const auto property0 = container->InsertItem<PropertyItem>(TagIndex::Append());
  const auto property1 = container->InsertItem<PropertyItem>(TagIndex::Append());

  TestViewModel viewmodel(m_model.get());

  // the model contains only the root item
  const auto root_index = viewmodel.indexFromItem(viewmodel.rootItem());
  const auto root_item_index = viewmodel.index(0, 0, root_index);

  EXPECT_EQ(viewmodel.rowCount(root_item_index), 0);

  EXPECT_EQ(viewmodel.FindViews(container).size(), 0);
  EXPECT_EQ(viewmodel.FindViews(property0).size(), 0);
  EXPECT_EQ(viewmodel.FindViews(property1).size(), 0);

  // insert container into the model
  EXPECT_NO_THROW(m_model->InsertItem(std::move(container_item), root_item, TagIndex::Append()));

  // the model contains the root item and the container children but not the container itself
  EXPECT_EQ(viewmodel.rowCount(root_item_index), 2);

  EXPECT_EQ(viewmodel.FindViews(container).size(), 0);

  EXPECT_NE(viewmodel.FindViews(property0).size(), 0);
  EXPECT_NE(viewmodel.FindViews(property1).size(), 0);

  const auto property0_index = viewmodel.index(0, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property0_index), property0);

  const auto property1_index = viewmodel.index(1, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property1_index), property1);
}

TEST_F(VirtualParentTests, RemoveVirtualParent)
{
  const auto root_item = m_model->InsertItem<TestItem>();

  const auto container = m_model->InsertItem<ContainerItem>(root_item);

  const auto property0 = m_model->InsertItem<PropertyItem>(container);
  const auto property1 = m_model->InsertItem<PropertyItem>(container);

  TestViewModel viewmodel(m_model.get());

  const auto root_index = viewmodel.indexFromItem(viewmodel.rootItem());
  const auto root_item_index = viewmodel.index(0, 0, root_index);

  // the model contains the root item and the container children but not the container itself
  EXPECT_EQ(viewmodel.rowCount(root_item_index), 2);

  EXPECT_EQ(viewmodel.FindViews(container).size(), 0);

  EXPECT_NE(viewmodel.FindViews(property0).size(), 0);
  EXPECT_NE(viewmodel.FindViews(property1).size(), 0);

  const auto property0_index = viewmodel.index(0, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property0_index), property0);

  const auto property1_index = viewmodel.index(1, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property1_index), property1);

  // remove container from the model
  EXPECT_NO_THROW(m_model->RemoveItem(container));

  // the model contains only the root item
  EXPECT_EQ(viewmodel.rowCount(root_item_index), 0);

  EXPECT_EQ(viewmodel.FindViews(container).size(), 0);
  EXPECT_EQ(viewmodel.FindViews(property0).size(), 0);
  EXPECT_EQ(viewmodel.FindViews(property1).size(), 0);
}

TEST_F(VirtualParentTests, InsertChildOfVirtualParent)
{
  const auto root_item = m_model->InsertItem<TestItem>();

  const auto container = m_model->InsertItem<ContainerItem>(root_item);

  const auto property0 = m_model->InsertItem<PropertyItem>(container);

  TestViewModel viewmodel(m_model.get());

  const auto root_index = viewmodel.indexFromItem(viewmodel.rootItem());
  const auto root_item_index = viewmodel.index(0, 0, root_index);

  // the model contains the root item and the container children but not the container itself
  EXPECT_EQ(viewmodel.rowCount(root_item_index), 1);

  EXPECT_EQ(viewmodel.FindViews(container).size(), 0);

  EXPECT_NE(viewmodel.FindViews(property0).size(), 0);

  const auto property0_index = viewmodel.index(0, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property0_index), property0);

  // insert new item into container
  const auto property1 = m_model->InsertItem<PropertyItem>(container);

  EXPECT_EQ(viewmodel.rowCount(root_item_index), 2);

  EXPECT_EQ(viewmodel.FindViews(container).size(), 0);

  EXPECT_NE(viewmodel.FindViews(property1).size(), 0);

  const auto property1_index = viewmodel.index(1, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property1_index), property1);
}

TEST_F(VirtualParentTests, RemoveParentOfVirtualParent)
{
  const auto root_item = m_model->InsertItem<TestItem>();

  const auto container = m_model->InsertItem<ContainerItem>(root_item);

  const auto property0 = m_model->InsertItem<PropertyItem>(container);
  const auto property1 = m_model->InsertItem<PropertyItem>(container);

  TestViewModel viewmodel(m_model.get());

  const auto root_index = viewmodel.indexFromItem(viewmodel.rootItem());
  const auto root_item_index = viewmodel.index(0, 0, root_index);

  // the model contains the root item and the container children but not the container itself
  EXPECT_EQ(viewmodel.rowCount(root_item_index), 2);

  EXPECT_EQ(viewmodel.FindViews(container).size(), 0);

  EXPECT_NE(viewmodel.FindViews(property0).size(), 0);
  EXPECT_NE(viewmodel.FindViews(property1).size(), 0);

  const auto property0_index = viewmodel.index(0, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property0_index), property0);

  const auto property1_index = viewmodel.index(1, 0, root_item_index);
  EXPECT_EQ(viewmodel.GetSessionItemFromIndex(property1_index), property1);

  // remove container from the model
  EXPECT_NO_THROW(m_model->RemoveItem(root_item));

  // the model contains no items
  EXPECT_EQ(viewmodel.rowCount(), 0);

  EXPECT_EQ(viewmodel.FindViews(root_item).size(), 0);
  EXPECT_EQ(viewmodel.FindViews(container).size(), 0);
  EXPECT_EQ(viewmodel.FindViews(property0).size(), 0);
  EXPECT_EQ(viewmodel.FindViews(property1).size(), 0);
}
