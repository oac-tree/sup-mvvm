/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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
#include <mvvm/standarditems/vector_item.h>

namespace
{

bool RegisterToyItems()
{
  mvvm::RegisterGlobalItem<mvvm::test::toyitems::ParticleItem>();
  mvvm::RegisterGlobalItem<mvvm::test::toyitems::LayerItem>();
  mvvm::RegisterGlobalItem<mvvm::test::toyitems::MultiLayerItem>();
  return true;
}

bool toy_items_registered_flag = RegisterToyItems();

}  // namespace

namespace mvvm::test::toyitems
{

ParticleItem::ParticleItem() : CompoundItem(mvvm::GetTypeName<ParticleItem>())
{
  AddProperty<mvvm::VectorItem>("position");
  AddProperty("shape", "sphere");
}

std::unique_ptr<SessionItem> ParticleItem::Clone() const
{
  return std::make_unique<ParticleItem>(*this);
}

LayerItem::LayerItem() : mvvm::CompoundItem(mvvm::GetTypeName<LayerItem>())
{
  AddProperty("Thickness", 42.0);
  AddProperty("Color", "green");
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kParticleTag, {mvvm::GetTypeName<ParticleItem>()}),
              /*set_as_default*/ true);
}

std::unique_ptr<SessionItem> LayerItem::Clone() const
{
  return std::make_unique<LayerItem>(*this);
}

MultiLayerItem::MultiLayerItem() : mvvm::CompoundItem(mvvm::GetTypeName<MultiLayerItem>())
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kLayerTag, {mvvm::GetTypeName<LayerItem>()}),
              /*set_as_default*/ true);
}

std::vector<LayerItem *> MultiLayerItem::GetLayers() const
{
  return GetItems<LayerItem>(kLayerTag);
}

std::unique_ptr<SessionItem> MultiLayerItem::Clone() const
{
  return std::make_unique<MultiLayerItem>(*this);
}

SequenceItem::SequenceItem() : NodeItem(mvvm::GetTypeName<SequenceItem>())
{
  RegisterMultipleChildrenTag();
}

RepeatItem::RepeatItem() : NodeItem(mvvm::GetTypeName<RepeatItem>())
{
  RegisterSingleChildTag();
}

WaitItem::WaitItem() : NodeItem(mvvm::GetTypeName<WaitItem>())
{
}

SampleModel::SampleModel() : mvvm::ApplicationModel("SampleModel") {}

}  // namespace mvvm::test::toyitems
