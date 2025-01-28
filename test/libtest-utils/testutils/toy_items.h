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

#ifndef LIBTEST_UTILS_TESTUTILS_TOY_ITEMS_H_
#define LIBTEST_UTILS_TESTUTILS_TOY_ITEMS_H_

//! @file
//! Collection of toy items for testing purpose.

#include <mvvm/model/application_model.h>
#include <mvvm/model/compound_item.h>

namespace mvvm::test::toyitems
{

const std::string kParticleTag = "Particles";
const std::string kLayerTag = "Layers";

class ParticleItem : public mvvm::CompoundItem
{
public:
  ParticleItem();
  static std::string GetStaticType();
};

class LayerItem : public mvvm::CompoundItem
{
public:
  LayerItem();
  static std::string GetStaticType();
};

class MultiLayerItem : public mvvm::CompoundItem
{
public:
  MultiLayerItem();
  static std::string GetStaticType();

  std::vector<LayerItem*> GetLayers() const;
};

class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();
};

}  // namespace mvvm::test::toyitems

#endif  // LIBTEST_UTILS_TESTUTILS_TOY_ITEMS_H_
