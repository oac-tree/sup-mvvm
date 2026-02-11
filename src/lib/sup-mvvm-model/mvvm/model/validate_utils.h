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

#ifndef MVVM_MODEL_VALIDATE_UTILS_H_
#define MVVM_MODEL_VALIDATE_UTILS_H_

//! @file
//! Collection of utility function to check if certain operations on the model are valid.

#include <mvvm/core/mvvm_errors.h>

#include <string>

namespace mvvm
{
class ISessionModel;
class SessionItem;
class TagIndex;
}  // namespace mvvm

namespace mvvm::utils
{

/**
 * @brief Converts TagIndex::Append() to the actual TagIndex.
 */
TagIndex GetInsertTagIndex(const SessionItem* parent, const TagIndex& tag_index);

/**
 * @brief Returns a error code for potential insert item operation.
 */
std::error_code GetInsertItemErrorCode(const SessionItem* item, const SessionItem* parent,
                                       const TagIndex& tag_index);

/**
 * @brief Perform validation if item insert is allowed.
 *
 * Will throw InvalidInsertException otherwise.
 */
void ValidateItemInsert(const SessionItem* item, const SessionItem* parent,
                        const TagIndex& tag_index);

/**
 * @brief Returns a error code for potential insert of a new item of the given type.
 */
std::error_code GetInsertTypeErrorCode(const std::string& item_type, const SessionItem* parent,
                                       const TagIndex& tag_index);

/**
 * @brief Perform validation if insert of the new item of the given type insert is allowed.
 *
 * Will throw InvalidInsertException otherwise.
 */
void ValidateItemTypeInsert(const std::string& item_type, const SessionItem* parent,
                            const TagIndex& tag_index);

/**
 * @brief Returns a error code for potential move operation.
 */
std::error_code GetMoveItemErrorCode(const SessionItem* item, const SessionItem* new_parent,
                                     const TagIndex& tag_index);

/**
 * @brief Perform validation if item move is allowed. Will throw InvalidMoveException otherwise.
 */
void ValidateItemMove(const SessionItem* item, const SessionItem* new_parent,
                      const TagIndex& tag_index);

/**
 * @brief Returns a error code for potential take item operation.
 */
std::error_code GetTakeItemErrorCode(const SessionItem& parent, const TagIndex& tag_index);

/**
 * @brief Perform validation if item take is allowed.
 *
 * Will throw InvalidOperationException otherwise.
 */
void ValidateTakeItem(const SessionItem& parent, const TagIndex& tag_index);

}  // namespace mvvm::utils

#endif  // MVVM_MODEL_VALIDATE_UTILS_H_
