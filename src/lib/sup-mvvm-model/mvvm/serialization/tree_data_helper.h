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

#ifndef MVVM_SERIALIZATION_TREE_DATA_HELPER_H_
#define MVVM_SERIALIZATION_TREE_DATA_HELPER_H_

#include <mvvm/serialization/tree_data_fwd.h>

#include <memory>
#include <string>
#include <vector>

namespace mvvm
{

std::vector<std::string> GetAttributeNames(const tree_data_t& tree_data);

std::unique_ptr<tree_data_t> ParseXMLElementString(const std::string& element);

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREE_DATA_HELPER_H_
