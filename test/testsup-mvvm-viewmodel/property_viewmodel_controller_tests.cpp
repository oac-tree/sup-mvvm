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

#include "mvvm/providers/property_viewmodel_controller.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/model/make_clone.h>
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/group_item.h>
#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <gtest/gtest.h>
#include <testutils/toy_items.h>

#include <QPersistentModelIndex>
#include <memory>

namespace mvvm::test
{

//! Tests for PropertyViewModelController class.
class PropertyViewModelControllerTests : public ::testing::Test
{
public:
  class TestViewModel : public ViewModel
  {
  public:
    void SetControllerIntern(std::unique_ptr<AbstractViewModelController> controller)
    {
      SetController(std::move(controller));
    }
  };

  //! Shape without any properties.
  class NoPropShapeItem : public CompoundItem
  {
  public:
    NoPropShapeItem() : CompoundItem("NoProp") {}
    std::unique_ptr<SessionItem> Clone() const override { return MakeClone(*this); }
  };

  //! Shape with a single property.
  class OnePropShapeItem : public CompoundItem
  {
  public:
    OnePropShapeItem() : CompoundItem("OneProp") { AddProperty("Radius", 5.0); }
    std::unique_ptr<SessionItem> Clone() const override { return MakeClone(*this); }
  };

  //! Group with an empty-property item selected by default.
  class TestGroupItem : public GroupItem
  {
  public:
    TestGroupItem() : GroupItem("TestGroup")
    {
      AddToGroup<NoPropShapeItem>();  // selected by default (index 0), no properties
      AddToGroup<OnePropShapeItem>();
    }
    std::unique_ptr<SessionItem> Clone() const override { return MakeClone(*this); }
  };

  //! Compound holding a group as a property.
  class HolderItem : public CompoundItem
  {
  public:
    HolderItem() : CompoundItem("Holder") { AddProperty<TestGroupItem>("group"); }
    std::unique_ptr<SessionItem> Clone() const override { return MakeClone(*this); }
  };

  ApplicationModel m_model;
  TestViewModel m_view_model;
};

//! Controller built on top of an empty model.
TEST_F(PropertyViewModelControllerTests, InitialState)
{
  m_view_model.SetControllerIntern(
      std::make_unique<PropertyViewModelController>(&m_model, &m_view_model));

  // root item has no properties
  EXPECT_EQ(m_view_model.rowCount(QModelIndex()), 0);
  EXPECT_EQ(m_view_model.columnCount(QModelIndex()), 2);
  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(QModelIndex()), m_model.GetRootItem());
}

//! Controller shows only property items and hides children stored in a universal tag.
TEST_F(PropertyViewModelControllerTests, ShowsOnlyProperties)
{
  auto parent = m_model.InsertItem<CompoundItem>();
  auto& property = parent->AddProperty("thickness", 0.0);

  // a non-property child added into a universal tag
  parent->RegisterTag(TagInfo::CreateUniversalTag("universal_tag"));
  m_model.InsertItem<SessionItem>(parent, TagIndex::Append("universal_tag"));

  m_view_model.SetControllerIntern(
      std::make_unique<PropertyViewModelController>(&m_model, &m_view_model));
  m_view_model.SetRootSessionItem(parent);

  // only the property item is visible (the universal-tag child is hidden)
  ASSERT_EQ(m_view_model.rowCount(QModelIndex()), 1);
  EXPECT_EQ(m_view_model.columnCount(QModelIndex()), 2);

  const QModelIndex label_index = m_view_model.index(0, 0, QModelIndex());
  const QModelIndex data_index = m_view_model.index(0, 1, QModelIndex());
  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(label_index), &property);
  EXPECT_EQ(m_view_model.GetSessionItemFromIndex(data_index), &property);
}

//! Controller created with a null model leaves the view model empty.
TEST_F(PropertyViewModelControllerTests, NullModel)
{
  m_view_model.SetControllerIntern(
      std::make_unique<PropertyViewModelController>(nullptr, &m_view_model));

  EXPECT_EQ(m_view_model.rowCount(QModelIndex()), 0);
  EXPECT_EQ(m_view_model.columnCount(QModelIndex()), 2);
}

//! A GroupItem shows the property items of its current item directly beneath itself.
TEST_F(PropertyViewModelControllerTests, GroupItemInitialState)
{
  using namespace mvvm::test::toyitems;

  auto particle = m_model.InsertItem<ParticleItem>();
  auto* group = particle->FindItem<ShapeGroupItem>(ParticleItem::kShapeTag);

  PropertyViewModel view_model(&m_model);
  view_model.SetRootSessionItem(particle);

  // particle has two properties: position (VectorItem) and shapes (group)
  ASSERT_EQ(view_model.rowCount(QModelIndex()), 2);

  // the group is the second property
  const QModelIndex group_index = view_model.index(1, 0, QModelIndex());
  EXPECT_EQ(view_model.GetSessionItemFromIndex(group_index), group);

  // the default selection is SphereItem, which has a single property (Radius)
  EXPECT_EQ(group->GetCurrentType(), GetTypeName<SphereItem>());
  EXPECT_EQ(view_model.rowCount(group_index), 1);
}

//! Changing the current item of a group regenerates the branch beneath the group.
TEST_F(PropertyViewModelControllerTests, GroupItemBranchRebuiltOnSelectionChange)
{
  using namespace mvvm::test::toyitems;

  auto particle = m_model.InsertItem<ParticleItem>();
  auto* group = particle->FindItem<ShapeGroupItem>(ParticleItem::kShapeTag);

  PropertyViewModel view_model(&m_model);
  view_model.SetRootSessionItem(particle);

  // initially the group shows Sphere's single property
  EXPECT_EQ(view_model.rowCount(view_model.index(1, 0, QModelIndex())), 1);

  // switching to a cylinder (Radius + Height)
  group->SetCurrentType(GetTypeName<CylinderItem>());
  {
    const QModelIndex group_index = view_model.index(1, 0, QModelIndex());
    // the group itself stays at the same place
    EXPECT_EQ(view_model.GetSessionItemFromIndex(group_index), group);
    EXPECT_EQ(view_model.rowCount(group_index), 2);
  }

  // switching to a pyramid (Length, Width, Height, Alpha)
  group->SetCurrentType(GetTypeName<AnysoPyramidItem>());
  {
    const QModelIndex group_index = view_model.index(1, 0, QModelIndex());
    EXPECT_EQ(view_model.GetSessionItemFromIndex(group_index), group);
    EXPECT_EQ(view_model.rowCount(group_index), 4);
  }

  // switching back to a sphere
  group->SetCurrentType(GetTypeName<SphereItem>());
  {
    const QModelIndex group_index = view_model.index(1, 0, QModelIndex());
    EXPECT_EQ(view_model.GetSessionItemFromIndex(group_index), group);
    EXPECT_EQ(view_model.rowCount(group_index), 1);
  }
}

//! The currently selected property item is the one reported beneath the group.
TEST_F(PropertyViewModelControllerTests, GroupItemBranchContent)
{
  using namespace mvvm::test::toyitems;

  auto particle = m_model.InsertItem<ParticleItem>();
  auto* group = particle->FindItem<ShapeGroupItem>(ParticleItem::kShapeTag);
  group->SetCurrentType(GetTypeName<CylinderItem>());

  PropertyViewModel view_model(&m_model);
  view_model.SetRootSessionItem(particle);

  const QModelIndex group_index = view_model.index(1, 0, QModelIndex());
  ASSERT_EQ(view_model.rowCount(group_index), 2);

  auto* cylinder = &group->GetCurrentItem();
  const QModelIndex radius_index = view_model.index(0, 0, group_index);
  const QModelIndex height_index = view_model.index(1, 0, group_index);
  EXPECT_EQ(view_model.GetSessionItemFromIndex(radius_index),
            cylinder->FindItem(CylinderItem::kRadiusTag));
  EXPECT_EQ(view_model.GetSessionItemFromIndex(height_index),
            cylinder->FindItem(CylinderItem::kHeightTag));
}

//! The group's own row is kept in place across a selection change; only its children are rebuilt.
//! A persistent index pointing to the group stays valid, while a persistent index pointing to an
//! old child gets invalidated.
TEST_F(PropertyViewModelControllerTests, GroupItemRowIsPreserved)
{
  using namespace mvvm::test::toyitems;

  auto particle = m_model.InsertItem<ParticleItem>();
  auto* group = particle->FindItem<ShapeGroupItem>(ParticleItem::kShapeTag);

  PropertyViewModel view_model(&m_model);
  view_model.SetRootSessionItem(particle);

  const QPersistentModelIndex group_index(view_model.index(1, 0, QModelIndex()));
  const QPersistentModelIndex old_child_index(view_model.index(0, 0, group_index));
  ASSERT_TRUE(group_index.isValid());
  ASSERT_TRUE(old_child_index.isValid());

  group->SetCurrentType(GetTypeName<CylinderItem>());

  // the group row itself was not removed/recreated
  EXPECT_TRUE(group_index.isValid());
  EXPECT_EQ(view_model.GetSessionItemFromIndex(group_index), group);

  // the previously shown child row is gone
  EXPECT_FALSE(old_child_index.isValid());
}

//! Reproduces the example scenario: a group whose default current item has no properties, shown
//! through the PropertyTreeView path (view model built with a null model, then SetRootSessionItem).
//! Switching to an item with a property must show that property beneath the group.
TEST_F(PropertyViewModelControllerTests, GroupItemBranchRebuiltFromEmpty)
{
  auto holder = m_model.InsertItem<HolderItem>();
  auto* group = holder->FindItem<TestGroupItem>("group");

  // mirror PropertyTreeView: null model at construction, root set afterwards
  PropertyViewModel view_model(nullptr);
  view_model.SetRootSessionItem(holder);

  const QModelIndex group_index = view_model.index(0, 0, QModelIndex());
  ASSERT_EQ(view_model.GetSessionItemFromIndex(group_index), group);

  // the default current item (NoProp) has no properties
  EXPECT_EQ(view_model.rowCount(group_index), 0);

  // switching to the item with a property
  group->SetCurrentType("OneProp");
  {
    const QModelIndex updated_index = view_model.index(0, 0, QModelIndex());
    EXPECT_EQ(view_model.GetSessionItemFromIndex(updated_index), group);
    EXPECT_EQ(view_model.rowCount(updated_index), 1);
  }
}

}  // namespace mvvm::test
