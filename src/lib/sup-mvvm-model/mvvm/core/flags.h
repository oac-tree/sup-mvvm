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

#ifndef MVVM_CORE_FLAGS_H_
#define MVVM_CORE_FLAGS_H_

#include <bitset>
#include <cstdint>
#include <type_traits>

namespace mvvm
{

/**
 * @brief The Flags class is a simple container for flags defined by the enum class.
 *
 * @tparam EnumT Enum class type.
 * @note Inspired by https://m-peko.github.io/craft-cpp/posts/different-ways-to-define-binary-flags/
 */
template <typename EnumT>
class Flags
{
  static_assert(std::is_enum_v<EnumT>, "Flags can only be specialized for enum types");

  using UnderlyingT = typename std::make_unsigned_t<typename std::underlying_type_t<EnumT>>;

public:
  Flags& set(EnumT e, bool value = true) noexcept
  {
    bits_.set(underlying(e), value);
    return *this;
  }

  Flags& reset(EnumT e) noexcept
  {
    set(e, false);
    return *this;
  }

  Flags& reset() noexcept
  {
    bits_.reset();
    return *this;
  }

  /**
   * @brief Checks if all of the bits are set to true.
   */
  [[nodiscard]] bool all() const noexcept { return bits_.all(); }

  /**
   * @brief Checks if any of the bits are set to true.
   */
  [[nodiscard]] bool any() const noexcept { return bits_.any(); }

  /**
   * @brief Checks if none of the bits are set to true.
   */
  [[nodiscard]] bool none() const noexcept { return bits_.none(); }

  /**
   * @brief Returns number of bits the bitset holds.
   */
  [[nodiscard]] constexpr std::size_t size() const noexcept { return bits_.size(); }

  /**
   * @brief Returns number of bits set to true.
   */
  [[nodiscard]] std::size_t count() const noexcept { return bits_.count(); }

  constexpr bool operator[](EnumT e) const { return bits_[underlying(e)]; }

  /**
   * @brief Converts the content of bitset to unsigned integer.
   */
  std::uint32_t to_ulong() const { return bits_.to_ulong(); }

private:
  static constexpr UnderlyingT underlying(EnumT e) { return static_cast<UnderlyingT>(e); }

  std::bitset<underlying(EnumT::size)> bits_;
};

}  // namespace mvvm

#endif  // MVVM_CORE_BASIC_SCALAR_TYPES_H_
