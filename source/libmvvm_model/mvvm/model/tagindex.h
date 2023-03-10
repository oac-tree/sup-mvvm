/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

//! Aggregate to hold (tag, index) information for SessionModel.

class MVVM_MODEL_EXPORT TagIndex
{
public:
  std::string tag = {};
  int index = 0;

  TagIndex() {}

  TagIndex(const std::string& name, int index = 0) : tag(name), index(index) {}
  TagIndex(const char* name, int index = 0) : tag(name), index(index) {}

  TagIndex Next() const;

  TagIndex Prev() const;

  static TagIndex Append(const std::string& tag_name = {});

  static TagIndex Prepend(const std::string& tag_name = {});

  bool operator==(const TagIndex& other) const;
  bool operator!=(const TagIndex& other) const;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_TAGINDEX_H_
