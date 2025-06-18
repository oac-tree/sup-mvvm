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

//! @file
//! Collection of helper methods related to the TreeData usage.

#include <mvvm/serialization/tree_data_fwd.h>

#include <memory>
#include <string>
#include <vector>

namespace mvvm
{

/**
 * @brief Returns list of attribute names.
 */
std::vector<std::string> GetAttributeNames(const tree_data_t& tree_data);

/**
 * @brief Parses string containing XML element representation into TreeData object.
 */
std::unique_ptr<tree_data_t> ParseXMLElementString(const std::string& element);

/**
 * @brief Checks if given TreeData represents SessionItemContainer serialization.
 */
bool IsItemContainerConvertible(const tree_data_t &tree_data);

/**
 * @brief Checks if given TreeData represents TaggedItems serialization.
 */
bool IsTaggedItemsConvertible(const tree_data_t& tree_data);

}  // namespace mvvm

#endif  // MVVM_SERIALIZATION_TREE_DATA_HELPER_H_
