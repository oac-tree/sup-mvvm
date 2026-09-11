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

#include "toy_items.h"

#include <mvvm/model/item_factory.h>
#include <mvvm/model/make_clone.h>
#include <mvvm/standarditems/group_item.h>
#include <mvvm/standarditems/vector_item.h>

namespace
{

bool RegisterToyItems()
{
  mvvm::RegisterGlobalItem<mvvm::test::toyitems::CylinderItem>();
  mvvm::RegisterGlobalItem<mvvm::test::toyitems::SphereItem>();
  mvvm::RegisterGlobalItem<mvvm::test::toyitems::AnysoPyramidItem>();
  mvvm::RegisterGlobalItem<mvvm::test::toyitems::ShapeGroupItem>();
  mvvm::RegisterGlobalItem<mvvm::test::toyitems::ParticleItem>();
  mvvm::RegisterGlobalItem<mvvm::test::toyitems::LayerItem>();
  mvvm::RegisterGlobalItem<mvvm::test::toyitems::MultiLayerItem>();
  return true;
}

bool toy_items_registered_flag = RegisterToyItems();

}  // namespace

namespace mvvm::test::toyitems
{

//! -----------------------------------------------------------------------------------------------
//! MultiLayer related
//! -----------------------------------------------------------------------------------------------

CylinderItem::CylinderItem() : CompoundItem(GetTypeName<CylinderItem>())
{
  AddProperty(kRadiusTag, 8.0);
  AddProperty(kHeightTag, 10.0);
}

std::unique_ptr<SessionItem> CylinderItem::Clone() const
{
  return MakeClone(*this);
}

SphereItem::SphereItem() : CompoundItem(GetTypeName<SphereItem>())
{
  AddProperty(kRadiusTag, 8.0);
}

std::unique_ptr<SessionItem> SphereItem::Clone() const
{
  return MakeClone(*this);
}

AnysoPyramidItem::AnysoPyramidItem() : CompoundItem(GetTypeName<AnysoPyramidItem>())
{
  AddProperty(kLengthTag, 8.0);
  AddProperty(kWidthTag, 8.0);
  AddProperty(kHeightTag, 8.0);
  AddProperty(kAlphaTag, 8.0);
}

std::unique_ptr<SessionItem> AnysoPyramidItem::Clone() const
{
  return MakeClone(*this);
}

ShapeGroupItem::ShapeGroupItem() : GroupItem(GetTypeName<ShapeGroupItem>())
{
  AddToGroup<CylinderItem>("Cylinder");
  AddToGroup<SphereItem>("Full sphere", /*make_selected*/ true);
  AddToGroup<AnysoPyramidItem>("Anysotropical pyramid");
}

std::unique_ptr<SessionItem> ShapeGroupItem::Clone() const
{
  return MakeClone(*this);
}

ParticleItem::ParticleItem() : CompoundItem(GetTypeName<ParticleItem>())
{
  AddProperty<VectorItem>(kPositionTag);
  AddProperty<ShapeGroupItem>(kShapeTag);
}

std::unique_ptr<SessionItem> ParticleItem::Clone() const
{
  return MakeClone(*this);
}

LayerItem::LayerItem() : CompoundItem(GetTypeName<LayerItem>())
{
  AddProperty("Thickness", 42.0);
  AddProperty("Color", "green");
  RegisterTag(TagInfo::CreateUniversalTag(kParticleTag, {GetTypeName<ParticleItem>()}),
              /*set_as_default*/ true);
}

std::unique_ptr<SessionItem> LayerItem::Clone() const
{
  return MakeClone(*this);
}

MultiLayerItem::MultiLayerItem() : CompoundItem(GetTypeName<MultiLayerItem>())
{
  RegisterTag(TagInfo::CreateUniversalTag(kLayerTag, {GetTypeName<LayerItem>()}),
              /*set_as_default*/ true);
}

std::unique_ptr<SessionItem> MultiLayerItem::Clone() const
{
  return MakeClone(*this);
}

std::vector<const LayerItem*> MultiLayerItem::GetLayers() const
{
  return FindItems<LayerItem>(kLayerTag);
}

std::vector<LayerItem*> MultiLayerItem::GetLayers()
{
  return FindItems<LayerItem>(kLayerTag);
}

//! -----------------------------------------------------------------------------------------------
//! Behavioral tree related
//! -----------------------------------------------------------------------------------------------

SequenceItem::SequenceItem() : NodeItem(GetTypeName<SequenceItem>())
{
  RegisterMultipleChildrenTag();
}

RepeatItem::RepeatItem() : NodeItem(GetTypeName<RepeatItem>())
{
  RegisterSingleChildTag();
}

WaitItem::WaitItem() : NodeItem(GetTypeName<WaitItem>()) {}

}  // namespace mvvm::test::toyitems
