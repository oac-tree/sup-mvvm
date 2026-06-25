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
#include <mvvm/model/property_item.h>
#include <mvvm/standarditems/group_item.h>
#include <mvvm/viewmodel/property_viewmodel.h>
#include <mvvm/viewmodel/viewmodel.h>

#include <gtest/gtest.h>
#include <testutils/toy_items.h>

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
  auto group = particle->GetItem<ShapeGroupItem>(ParticleItem::kShapeTag);

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
  auto group = particle->GetItem<ShapeGroupItem>(ParticleItem::kShapeTag);

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
  auto group = particle->GetItem<ShapeGroupItem>(ParticleItem::kShapeTag);
  group->SetCurrentType(GetTypeName<CylinderItem>());

  PropertyViewModel view_model(&m_model);
  view_model.SetRootSessionItem(particle);

  const QModelIndex group_index = view_model.index(1, 0, QModelIndex());
  ASSERT_EQ(view_model.rowCount(group_index), 2);

  auto cylinder = group->GetCurrentItem();
  const QModelIndex radius_index = view_model.index(0, 0, group_index);
  const QModelIndex height_index = view_model.index(1, 0, group_index);
  EXPECT_EQ(view_model.GetSessionItemFromIndex(radius_index),
            cylinder->GetItem(CylinderItem::kRadiusTag));
  EXPECT_EQ(view_model.GetSessionItemFromIndex(height_index),
            cylinder->GetItem(CylinderItem::kHeightTag));
}

}  // namespace mvvm::test
