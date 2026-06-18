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

#ifndef MVVM_CORE_FLAGS_H_
#define MVVM_CORE_FLAGS_H_

#include <mvvm/core/mvvm_exceptions.h>

#include <bitset>
#include <cstdint>
#include <type_traits>
#include <vector>

namespace mvvm
{

/**
 * @brief The Flags class is a simple container for flags defined by the enum class.
 *
 * @tparam EnumT Enum class type.
 *
 * @note Inspired by https://m-peko.github.io/craft-cpp/posts/different-ways-to-define-binary-flags/
 * It is required that the last element of enum class is called "size". It is not counted as
 * a flag and shouldn't be used.
 *
 * enum class Option : std::uint8_t { kLength, kHeight, size};
 * Flags<Option> flags;
 */
template <typename EnumT>
class Flags
{
  static_assert(std::is_enum_v<EnumT>, "Flags can only be specialized for enum types");

public:
  using UnderlyingT = typename std::make_unsigned_t<typename std::underlying_type_t<EnumT>>;
  using StorageT = std::uint64_t;

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
   * @brief Constructs a bitset and sets given flags to true.
   */
  explicit Flags(const std::vector<EnumT>& values);

  /**
   * @brief Set bits to true or given value.
   */
  Flags& Set(EnumT flag, bool value = true);

  /**
   * @brief Toggle bits.
   */
  Flags& Toggle(EnumT flag);

  /**
   * @brief Set bits to false.
   */
  Flags& Reset(EnumT flag);

  /**
   * @brief Set all bits to false.
   */
  Flags& Reset() noexcept;

  /**
   * @brief Checks if All of the bits are set to true.
   */
  [[nodiscard]] bool All() const noexcept;

  /**
   * @brief Checks if Any of the bits are set to true.
   */
  [[nodiscard]] bool Any() const noexcept;

  /**
   * @brief Checks if None of the bits are set to true.
   */
  [[nodiscard]] bool None() const noexcept;

  /**
   * @brief Returns number of bits the bitset holds.
   */
  [[nodiscard]] constexpr std::size_t GetSize() const noexcept;

  /**
   * @brief Returns number of bits set to true.
   */
  [[nodiscard]] std::size_t Count() const noexcept;

  /**
   * @brief Accesses the value of bit.
   */
  constexpr bool operator[](EnumT flag) const;

  /**
   * @brief Checks if the bit is set.
   *
   * Same behavior as [index] operator.
   */
  constexpr bool IsSet(EnumT flag) const;

  /**
   * @brief Converts the content of bitset to unsigned integer.
   */
  StorageT to_ulong() const { return m_bits.to_ulong(); }

private:
  static constexpr UnderlyingT underlying(EnumT flag) { return static_cast<UnderlyingT>(flag); }

  std::bitset<underlying(EnumT::size)> m_bits;
};

template <typename EnumT>
inline Flags<EnumT>& Flags<EnumT>::Set(EnumT flag, bool value)
{
  const auto index = static_cast<std::size_t>(underlying(flag));

  if (index >= m_bits.size())
  {
    throw RuntimeException("Flag index is out of range");
  }

  (void)m_bits.set(index, value);
  return *this;
}

template <typename EnumT>
inline Flags<EnumT>& Flags<EnumT>::Toggle(EnumT flag)
{
  const auto index = static_cast<std::size_t>(underlying(flag));

  if (index >= m_bits.size())
  {
    throw RuntimeException("Flag index is out of range");
  }

  (void)m_bits.flip(index);
  return *this;
}

template <typename EnumT>
inline Flags<EnumT>& Flags<EnumT>::Reset(EnumT flag)
{
  (void)Set(flag, false);
  return *this;
}

template <typename EnumT>
inline Flags<EnumT>& Flags<EnumT>::Reset() noexcept
{
  (void)m_bits.reset();
  return *this;
}

template <typename EnumT>
inline Flags<EnumT>::Flags(std::uint32_t value) : m_bits(value)
{
}

template <typename EnumT>
inline Flags<EnumT>::Flags(const std::vector<EnumT>& values)
{
  for (auto value : values)
  {
    (void)Set(value);
  }
}

template <typename EnumT>
inline bool Flags<EnumT>::All() const noexcept
{
  return m_bits.all();
}

template <typename EnumT>
inline bool Flags<EnumT>::Any() const noexcept
{
  return m_bits.any();
}

template <typename EnumT>
inline bool Flags<EnumT>::None() const noexcept
{
  return m_bits.none();
}

template <typename EnumT>
constexpr std::size_t Flags<EnumT>::GetSize() const noexcept
{
  return m_bits.size();
}

template <typename EnumT>
inline std::size_t Flags<EnumT>::Count() const noexcept
{
  return m_bits.count();
}

template <typename EnumT>
constexpr bool Flags<EnumT>::operator[](EnumT flag) const
{
  const auto index = static_cast<std::size_t>(underlying(flag));

  if (index >= m_bits.size())
  {
    throw RuntimeException("Flag index is out of range");
  }

  return m_bits[index];
}

template <typename EnumT>
constexpr bool Flags<EnumT>::IsSet(EnumT flag) const
{
  const auto index = static_cast<std::size_t>(underlying(flag));

  if (index >= m_bits.size())
  {
    throw RuntimeException("Flag index is out of range");
  }

  return m_bits[index];
}

template <typename EnumT>
bool operator==(const Flags<EnumT>& lhs, const Flags<EnumT>& rhs)
{
  return lhs.to_ulong() == rhs.to_ulong();
}

template <typename EnumT>
bool operator!=(const Flags<EnumT>& lhs, const Flags<EnumT>& rhs)
{
  return !(lhs == rhs);
}

}  // namespace mvvm

#endif  // MVVM_CORE_FLAGS_H_
