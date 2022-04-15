/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef TESTS_LIBTESTMACHINERY_TOYITEMS_H
#define TESTS_LIBTESTMACHINERY_TOYITEMS_H

//! @file toyitems.h
//! Collection of toy items for testing purpose.

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/compounditem.h"

namespace testutils::toyitems
{

class ParticleItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "Particle";
  ParticleItem();
};

class LayerItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "Layer";
  LayerItem();
};

class MultiLayerItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "MultiLayer";
  MultiLayerItem();
};

class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();
};

}  // namespace testutils::toyitems

#endif  // TESTS_LIBTESTMACHINERY_TOYITEMS_H
