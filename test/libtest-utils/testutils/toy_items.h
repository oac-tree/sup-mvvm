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

#ifndef LIBTEST_UTILS_TESTUTILS_TOY_ITEMS_H_
#define LIBTEST_UTILS_TESTUTILS_TOY_ITEMS_H_

//! @file
//! Collection of toy items for testing purpose.

#include <mvvm/model/compound_item.h>
#include <mvvm/standarditems/node_item.h>

namespace mvvm::test::toyitems
{

/**
 * @brief The ParticleItem class represents a nano-particle with (x,y,z) coordinate and a shape.
 *
 * Used to describe a sample in scattering physics.
 */
class ParticleItem : public mvvm::CompoundItem
{
public:
  ParticleItem();
  std::unique_ptr<SessionItem> Clone() const override;
};

/**
 * @brief The LayerItem class represents a layer of some material, can contain embedded
 * nano-particles.
 *
 * Used to describe a sample in scattering physics.
 */
class LayerItem : public mvvm::CompoundItem
{
public:
  static constexpr auto kParticleTag = "Particles";

  LayerItem();
  std::unique_ptr<SessionItem> Clone() const override;
};

/**
 * @brief The MultiLayerItem class represent a multi-layered material made of layers.
 *
 * Used to describe a sample in scattering physics.
 */
class MultiLayerItem : public mvvm::CompoundItem
{
public:
  static constexpr auto kLayerTag = "Layers";

  MultiLayerItem();
  std::unique_ptr<SessionItem> Clone() const override;

  std::vector<LayerItem*> GetLayers() const;
};

/**
 * @brief The SequenceItem class represents a Sequence instruction.
 *
 * Used to build behavior tree.
 */
class SequenceItem : public mvvm::NodeItem
{
public:
  SequenceItem();
};

/**
 * @brief The RepeatItem class represents a Repeat instruction.
 *
 * Used to build behavior tree.
 */
class RepeatItem : public mvvm::NodeItem
{
public:
  RepeatItem();
};

/**
 * @brief The RepeatItem class represents a Wait instruction.
 *
 * Used to build behavior tree.
 */
class WaitItem : public mvvm::NodeItem
{
public:
  WaitItem();
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

template <>
struct mvvm::item_traits<mvvm::test::toyitems::SequenceItem>
{
  static constexpr std::string_view type_name() noexcept { return "SequenceItem"; }
};

template <>
struct mvvm::item_traits<mvvm::test::toyitems::RepeatItem>
{
  static constexpr std::string_view type_name() noexcept { return "RepeatItem"; }
};

template <>
struct mvvm::item_traits<mvvm::test::toyitems::WaitItem>
{
  static constexpr std::string_view type_name() noexcept { return "WaitItem"; }
};

#endif  // LIBTEST_UTILS_TESTUTILS_TOY_ITEMS_H_
