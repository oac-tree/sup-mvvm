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
  std::unique_ptr<SessionItem> Clone() const override;
};

class LayerItem : public mvvm::CompoundItem
{
public:
  LayerItem();
  std::unique_ptr<SessionItem> Clone() const override;
};

class MultiLayerItem : public mvvm::CompoundItem
{
public:
  MultiLayerItem();
  std::vector<LayerItem*> GetLayers() const;
  std::unique_ptr<SessionItem> Clone() const override;
};

class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();
};

}  // namespace mvvm::test::toyitems

template <>
struct mvvm::item_traits<mvvm::test::toyitems::ParticleItem>
{
  static constexpr std::string_view type_name() noexcept { return "Particle"; }
};

template <>
struct mvvm::item_traits<mvvm::test::toyitems::LayerItem>
{
  static constexpr std::string_view type_name() noexcept { return "Layer"; }
};

template <>
struct mvvm::item_traits<mvvm::test::toyitems::MultiLayerItem>
{
  static constexpr std::string_view type_name() noexcept { return "MultiLayer"; }
};

#endif  // LIBTEST_UTILS_TESTUTILS_TOY_ITEMS_H_
