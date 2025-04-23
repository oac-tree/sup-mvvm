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

#ifndef MVVM_MODEL_TAGINDEX_H_
#define MVVM_MODEL_TAGINDEX_H_

#include <mvvm/model_export.h>

#include <cstdint>
#include <string>

namespace mvvm
{

/**
 * @brief The TagIndex class is an aggregate to hold (tag, index) information for SessionModel.
 *
 * It encodes all information needed to find a child inside its parent. A tag is a name of the
 * container with items, and index is a position of an item in this container.
 */
class MVVM_MODEL_EXPORT TagIndex
{
public:
  /**
   * @brief A string constant used to address the container marked with the default flag.
   */
  static inline const std::string kDefaultTag = {};

  /**
   * @brief The default c-tor.
   *
   * It represents a TagIndex addressing the first element of the item container marked as default.
   */
  TagIndex() = default;

  /**
   * @brief The constructor to build TagIndex from name and index.
   *
   * Implicit conversion is allowed.
   */
  TagIndex(const std::string& name, std::size_t index = 0);

  /**
   * @brief The constructor to build TagIndex from name and index.
   *
   * Implicit conversion is allowed.
   */
  TagIndex(const char* name, std::size_t index = 0);

  /**
   * @brief Returns tag.
   */
  std::string GetTag() const;

  /**
   * @brief Returns index.
   */
  std::size_t GetIndex() const;

  /**
   * @brief Constructs a new TagIndex representing the following index in a given tag.
   *
   * No validity check.
   */
  TagIndex Next() const;

  /**
   * @brief Constructs a new TagIndex representing the previous index in a given tag.
   *
   * No validity check.
   */
  TagIndex Prev() const;

  /**
   * @brief Checks if TagIndex is in a valid state.
   *
   * A valid state is any TagIndex that can be used to try to address the position in the item
   * container. It is not guaranteed, however, that such a position exists. An invalid state is
   * normally used as a return value from functions, when a given item doesn't belong to the
   * container.
   */
  bool IsValid() const;

  /**
   * @brief Checks if TagIndex is intended for append operations.
   */
  bool IsAppend() const;

  /**
   * @brief Returns TagIndex corresponding to the end of the container with the given name.
   *
   * If the default tag name is used, the SessionModel will try to find a container marked as
   * default.
   */
  static TagIndex Append(const std::string& tag_name = kDefaultTag);

  /**
   * @brief Returns TagIndex corresponding to the first index in the container with the given name.
   *
   * If the default tag name is used, the SessionModel will try to find a container marked as
   * default.
   */
  static TagIndex First(const std::string& tag_name = kDefaultTag);

  /**
   * @brief Returns TagIndex corresponding to the given index in the container marked as a default.
   */
  static TagIndex Default(std::uint32_t item_index);

  /**
   * @brief Returns TagIndex corresponding to an invalid state.
   * @see TagIndex::IsValid()
   */
  static TagIndex Invalid();

  bool operator==(const TagIndex& other) const;
  bool operator!=(const TagIndex& other) const;

private:
  std::string m_tag = {};   //!< The name of the item container.
  std::size_t m_index = 0;  //!< The position in the item container.
};

}  // namespace mvvm

#endif  // MVVM_MODEL_TAGINDEX_H_
