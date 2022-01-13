/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_PATH_H
#define MVVM_MODEL_PATH_H

#include "mvvm/model_export.h"

#include <string>
#include <vector>

namespace mvvm
{
//! Supports navigation through SessionModel. Contains a chain of indexes that have to
//! be used to reach the desired SessionItem starting from the root item. Path class plays
//! a role of simplified QModelIndex for SessionModel. Used for undo/redo only.

//! Example of tree:
//! - root              path:""
//!   - child           path:"0"
//!     - grandchild    path:"0,0"
//!     - grandchild    path:"0,1"
//!   - child           path:"1"

class MVVM_MODEL_EXPORT Path
{
public:
  using PathElement = int;
  using container_t = std::vector<PathElement>;
  using iterator = container_t::iterator;
  using const_iterator = container_t::const_iterator;

  Path() = default;

  static Path CreateFromString(const std::string& str);

  static Path CreateFromVector(const std::vector<int>& data);

  std::string GetString() const;

  void Append(PathElement element);
  void Prepend(PathElement element);

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

private:
  container_t m_data;
};

}  // namespace ModelView

#endif  // MVVM_MODEL_PATH_H
