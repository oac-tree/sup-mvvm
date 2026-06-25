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

#include "sample_model.h"

#include <mvvm/standarditems/editor_constants.h>
#include <mvvm/standarditems/vector_item.h>

namespace groupproperty
{

UndefinedShapeItem::UndefinedShapeItem() : mvvm::CompoundItem("Undefined")
{
  // no properties
}

SphereItem::SphereItem() : mvvm::CompoundItem("Sphere")
{
  AddProperty("Radius", 5.0);
}

CylinderItem::CylinderItem() : mvvm::CompoundItem("Cylinder")
{
  AddProperty("Radius", 8.0);
  AddProperty("Height", 10.0);
}

BoxItem::BoxItem() : mvvm::CompoundItem("Box")
{
  AddProperty("Length", 12.0);
  AddProperty("Width", 6.0);
  AddProperty("Height", 3.0);
}

ShapeGroupItem::ShapeGroupItem() : mvvm::GroupItem("Shape")
{
  AddToGroup<UndefinedShapeItem>();
  AddToGroup<SphereItem>();
  AddToGroup<CylinderItem>();
  AddToGroup<BoxItem>();
}

ParticleItem::ParticleItem() : mvvm::CompoundItem("Particle")
{
  AddProperty(kColorTag, "green").SetEditorType(mvvm::constants::kColorEditorType);
  AddProperty<ShapeGroupItem>(kShapeTag);
  AddProperty<mvvm::VectorItem>(kPositionTag);
}

SampleModel::SampleModel() : mvvm::ApplicationModel("SampleModel") {}

}  // namespace groupproperty
