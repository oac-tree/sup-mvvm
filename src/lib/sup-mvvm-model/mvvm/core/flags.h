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
 *
 * @note Inspired by https://m-peko.github.io/craft-cpp/posts/different-ways-to-define-binary-flags/
 * It is required that the last element of enum class is called "size"
 *
 * enum class Option : std::uint8_t { kLength, kHeight, size};
 * Flags<Option> flags;
 */
template <typename EnumT>
class Flags
{
  static_assert(std::is_enum_v<EnumT>, "Flags can only be specialized for enum types");

  using UnderlyingT = typename std::make_unsigned_t<typename std::underlying_type_t<EnumT>>;

public:
  /**
   * @brief Default constructor.
   *
   * COnstruct a bitset with all bits set to zero.
   */
  Flags() = default;

  /**
   * @brief Constructs a bitset from an unsigned integer.
   */
  explicit Flags(std::uint32_t value);

  /**
   * @brief Set bits to true or given value.
   */
  Flags& set(EnumT flag, bool value = true) noexcept;

  /**
   * @brief Set bits to false.
   */
  Flags& reset(EnumT flag) noexcept;

  /**
   * @brief Set all bits to false.
   */
  Flags& reset() noexcept;

  /**
   * @brief Checks if all of the bits are set to true.
   */
  [[nodiscard]] bool all() const noexcept;

  /**
   * @brief Checks if any of the bits are set to true.
   */
  [[nodiscard]] bool any() const noexcept;

  /**
   * @brief Checks if none of the bits are set to true.
   */
  [[nodiscard]] bool none() const noexcept;

  /**
   * @brief Returns number of bits the bitset holds.
   */
  [[nodiscard]] constexpr std::size_t size() const noexcept;

  /**
   * @brief Returns number of bits set to true.
   */
  [[nodiscard]] std::size_t count() const noexcept;

  constexpr bool operator[](EnumT flag) const;

  /**
   * @brief Converts the content of bitset to unsigned integer.
   */
  std::uint32_t to_ulong() const { return m_bits.to_ulong(); }

private:
  static constexpr UnderlyingT underlying(EnumT flag) { return static_cast<UnderlyingT>(flag); }

  std::bitset<underlying(EnumT::size)> m_bits;
};

template <typename EnumT>
inline Flags<EnumT>& Flags<EnumT>::set(EnumT flag, bool value) noexcept
{
  m_bits.set(underlying(flag), value);
  return *this;
}

template <typename EnumT>
inline Flags<EnumT>& Flags<EnumT>::reset(EnumT flag) noexcept
{
  set(flag, false);
  return *this;
}

template <typename EnumT>
inline Flags<EnumT>& Flags<EnumT>::reset() noexcept
{
  m_bits.reset();
  return *this;
}

template <typename EnumT>
inline Flags<EnumT>::Flags(std::uint32_t value) : m_bits(value)
{
}

template <typename EnumT>
inline bool Flags<EnumT>::all() const noexcept
{
  return m_bits.all();
}

template <typename EnumT>
inline bool Flags<EnumT>::any() const noexcept
{
  return m_bits.any();
}

template <typename EnumT>
inline bool Flags<EnumT>::none() const noexcept
{
  return m_bits.none();
}

template <typename EnumT>
constexpr std::size_t Flags<EnumT>::size() const noexcept
{
  return m_bits.size();
}

template <typename EnumT>
inline std::size_t Flags<EnumT>::count() const noexcept
{
  return m_bits.count();
}

template <typename EnumT>
constexpr bool Flags<EnumT>::operator[](EnumT flag) const
{
  return m_bits[underlying(flag)];
}

}  // namespace mvvm

#endif  // MVVM_CORE_BASIC_SCALAR_TYPES_H_
