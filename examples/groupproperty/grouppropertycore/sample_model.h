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

#ifndef GROUPPROPERTY_GROUPPROPERTYCORE_SAMPLE_MODEL_H_
#define GROUPPROPERTY_GROUPPROPERTYCORE_SAMPLE_MODEL_H_

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>
#include <mvvm/standarditems/group_item.h>

namespace groupproperty
{

//! Represents an undefined shape (carries no parameters).

class UndefinedShapeItem : public mvvm::CompoundItem
{
public:
  UndefinedShapeItem();
};

//! Represents a sphere.

class SphereItem : public mvvm::CompoundItem
{
public:
  SphereItem();
};

//! Represents a cylindrical shape.

class CylinderItem : public mvvm::CompoundItem
{
public:
  CylinderItem();
};

//! Represents a box.

class BoxItem : public mvvm::CompoundItem
{
public:
  BoxItem();
};

//! Represents a group of possible shapes with only one of them being current at a time.

class ShapeGroupItem : public mvvm::GroupItem
{
public:
  ShapeGroupItem();
};

//! Represents a particle. Particle has a color, a shape, and a position.

class ParticleItem : public mvvm::CompoundItem
{
public:
  static constexpr auto kColorTag = "Color";
  static constexpr auto kShapeTag = "Shape";
  static constexpr auto kPositionTag = "Position";

  ParticleItem();
};

//! Main application model.

class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();
};

}  // namespace groupproperty

#endif  // GROUPPROPERTY_GROUPPROPERTYCORE_SAMPLE_MODEL_H_
