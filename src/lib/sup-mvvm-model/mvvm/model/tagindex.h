/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_TAGINDEX_H_
#define MVVM_MODEL_TAGINDEX_H_

#include <mvvm/model_export.h>

#include <string>

namespace mvvm
{

/**
 * @brief The TagIndex class is an aggregate to hold (tag, index) information for SessionModel.
 *
 * @details It encodes all information needed to find a child inside its parent. A tag is a
 * name of the container with items, and index is a position of an item in this container.
 */

class MVVM_MODEL_EXPORT TagIndex
{
public:
  /**
   * @brief kDefaultTag String constant is used to address the container which if marked with the
   * default flag.
   */
  static inline const std::string kDefaultTag = {};

  std::string tag = {};
  int index = 0;

  TagIndex() = default;

  /**
   * @brief Constructor to construct TagIndex from the given parameters. Implicit conversion is
   * allowed.
   */
  TagIndex(const std::string& name, int index = 0) : tag(name), index(index) {}

  /**
   * @brief Constructor to construct TagIndex from the given parameters. Implicit conversion is
   * allowed.
   */
  TagIndex(const char* name, int index = 0) : tag(name), index(index) {}

  /**
   * @brief Constructs new TagIndex representing following index in given tag. No validity check.
   */
  TagIndex Next() const;

  /**
   * @brief Constructs new TagIndex representing previous index in given tag. No validity check.
   */
  TagIndex Prev() const;

  /**
   * @brief Returns TagIndex corresponding to the end of the container with given name.
   * If the default name is used, will look for container marked as a default.
   */
  static TagIndex Append(const std::string& tag_name = kDefaultTag);

  /**
   * @brief Returns TagIndex corresponding to the beginning of the container with given name.
   * If the default name is used, will look for container marked as a default.
   */
  static TagIndex First(const std::string& tag_name = kDefaultTag);

  /**
   * @brief Returns TagIndex corresponding to the container marked as a default one and given index.
   */
  static TagIndex Default(int index);

  bool operator==(const TagIndex& other) const;
  bool operator!=(const TagIndex& other) const;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_TAGINDEX_H_
