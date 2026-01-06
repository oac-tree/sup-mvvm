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

#include "bin_utils.h"

#include <mvvm/utils/file_utils.h>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

namespace mvvm::utils
{

namespace
{
/**
 * @brief Returns a size of the bufer which will be used for file analysis.
 *
 * Will be 2048 bytes if file is large, or file size if file is smaller than 2048 bytes.
 */
std::size_t GetTestBufferSize(const std::string& filename)
{
  constexpr std::size_t kBuferSize = 2048;

  std::ifstream file_stream;
  file_stream.open(filename, std::ios_base::binary);
  (void)file_stream.seekg(0, std::ios_base::end);
  const std::size_t size = static_cast<std::size_t>(file_stream.tellg());
  file_stream.close();
  return (size > kBuferSize) ? kBuferSize : size;
}

/**
 * @brief Returns binary buffer containing first bytes of the file.
 */
std::vector<std::uint8_t> GetTestData(const std::string& filename)
{
  std::vector<std::uint8_t> result;

  const std::size_t byte_length = GetTestBufferSize(filename);

  result.resize(byte_length);
  std::ifstream fstr(filename, std::ios::in | std::ios::binary);
  for (std::size_t i = 0; i < byte_length; i++)
  {
    std::ifstream::char_type byte{};
    (void)fstr.read(&byte, 1);
    result[i] = byte;
  }
  return result;
}

/**
 * @brief Checks if character is a control character.
 */
bool IsControlChar(std::uint8_t symbol)
{
  // https://www.ascii-code.com
  // Decimals in the range [1, 7] and [14, 26] are control chars
  constexpr std::uint8_t kStartOfHeading = 0x01;
  constexpr std::uint8_t kBellAlert = 0x07;
  constexpr std::uint8_t kShiftOut = 0x0E;
  constexpr std::uint8_t kSubstitute = 0x1A;

  const bool is_in_range1 = (symbol >= kStartOfHeading) && (symbol <= kBellAlert);
  const bool is_in_range2 = (symbol >= kShiftOut) && (symbol <= kSubstitute);

  return is_in_range1 || is_in_range2;
}

/**
 * @brief Checks if there are two consecutive null characters in the buffer.
 */
bool ContainsConsequitiveNull(const std::vector<std::uint8_t>& buffer)
{
  constexpr std::uint8_t kNullChar = 0x00;

  for (std::size_t i = 1; i < buffer.size(); ++i)
  {
    if ((buffer[i] == kNullChar) && (buffer[i - 1] == kNullChar))
    {
      return true;
    }
  }
  return false;
}

}  // namespace

bool IsBinaryFile(const std::string& file_name)
{
  if (!utils::IsExists(file_name))
  {
    return false;
  }

  auto buffer = GetTestData(file_name);

  auto contains_control_char = std::any_of(
      buffer.begin(), buffer.end(), [](std::uint8_t element) { return IsControlChar(element); });

  return contains_control_char ? true : ContainsConsequitiveNull(buffer);
}

bool IsTextFile(const std::string& file_name)
{
  if (!utils::IsExists(file_name))
  {
    return false;
  }

  return !IsBinaryFile(file_name);
}

}  // namespace mvvm::utils
