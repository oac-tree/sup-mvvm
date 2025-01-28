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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
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

ParticleItem::ParticleItem() : CompoundItem(ParticleItem::GetStaticType())
{
  AddProperty<mvvm::VectorItem>("position");
  AddProperty("shape", "sphere");
}

std::string ParticleItem::GetStaticType()
{
  return "Particle";
}

LayerItem::LayerItem() : mvvm::CompoundItem(LayerItem::GetStaticType())
{
  AddProperty("Thickness", 42.0);
  AddProperty("Color", "green");
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kParticleTag, {ParticleItem::GetStaticType()}),
              /*set_as_default*/ true);
}

std::string LayerItem::GetStaticType()
{
  return "Layer";
}

MultiLayerItem::MultiLayerItem() : mvvm::CompoundItem(GetStaticType())
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kLayerTag, {LayerItem::GetStaticType()}),
              /*set_as_default*/ true);
}

std::string MultiLayerItem::GetStaticType()
{
  return "MultiLayer";
}

std::vector<LayerItem *> MultiLayerItem::GetLayers() const
{
  return GetItems<LayerItem>(kLayerTag);
}

SampleModel::SampleModel() : mvvm::ApplicationModel("SampleModel") {}

}  // namespace mvvm::test::toyitems
