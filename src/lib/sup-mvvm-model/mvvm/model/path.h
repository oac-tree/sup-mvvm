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

#ifndef MVVM_MODEL_PATH_H_
#define MVVM_MODEL_PATH_H_

#include <cstdint>
#include <string>
#include <vector>

namespace mvvm
{

/**
 * @brief The Path class supports navigation through SessionModel.
 *
 * Contains a chain of indexes that have to be used to reach the desired SessionItem starting
 * from the root item. The Path class serves as a simplified version of QModelIndex for
 * SessionModel.
 *
 * Example of tree:
 * - root              path:""
 *   - child           path:"0"
 *     - grandchild    path:"0,0"
 *     - grandchild    path:"0,1"
 *   - child           path:"1"
 */
class Path
{
public:
  using PathElement = std::uint32_t;
  using container_t = std::vector<PathElement>;
  using iterator = container_t::iterator;
  using const_iterator = container_t::const_iterator;

  Path() = default;

  /**
   * @brief Constructs Path object from string containing sequence of integers ("0,0,1,3").
   */
  static Path CreateFromString(const std::string& str);

  /**
   * @brief Constructs Path object from vector of integers.
   */
  static Path CreateFromVector(const std::vector<PathElement>& data);

  /**
   * @brief Returns string representing path ("0,0,1,3").
   */
  std::string GetString() const;

  /**
   * @brief Returns the length of the path (which corresponds to the depth level of the item).
   */
  std::size_t GetLength() const;

  void Append(PathElement element);
  void Prepend(PathElement element);

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

private:
  container_t m_data;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_PATH_H_
