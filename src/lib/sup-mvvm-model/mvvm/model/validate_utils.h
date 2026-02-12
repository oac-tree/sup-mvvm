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
#include <mvvm/model/tagindex.h>

#include <string>

namespace mvvm
{
class ISessionModel;
class SessionItem;
}  // namespace mvvm

namespace mvvm::utils
{

struct MoveInfo
{
  std::error_code error_code;  //! potential error code of move operation
  TagIndex target_index;       //! the index where the item will appear in the case of success
};

/**
 * @brief Returns verified TagIndex.
 *
 * The method is used to convert TagIndex::Append() to the actual insert index. Also performs
 * validation if index is valid and tag name exists. Will return a TagIndex in invalid state if it
 * is not the case.
 *
 * @param index A tag index for item insert.
 * @return A verified index.
 */
TagIndex GetVerifiedTagIndex(const SessionItem& parent, const TagIndex& tag_index);

/**
 * @brief Returns insert index which has to be used as a part of move operation.
 *
 * Since our move operation is a combination of take/insert operations, we have to adjust
 * insert tag depending on the original position of the item and the position where it is moved to.
 *
 * @param item An item to move.
 * @param new_parent A new parent for the item.
 * @param tag_index A tag index for the move operation.
 * @return
 */
TagIndex GetInsertTagIndexFromMove(const SessionItem& item, const SessionItem& parent,
                                   const TagIndex& tag_index);

/**
 * @brief Checks if move operation is a noop.
 *
 * It is a noop if item is moved to the same parent and the same tag index. This method checks only
 * if item is already at desired place. It doesn't check if move operation is valid. Used to avoid
 * unnecessary signaling.
 *
 * @param item An item to move.
 * @param new_parent A new parent for the item.
 * @param tag_index A tag index for the move operation.
 * @return True if move is a noop, false otherwise.
 */
bool IsNoopMove(const SessionItem& item, const SessionItem& new_parent, const TagIndex& tag_index);

/**
 * @brief Returns a error code for potential insert item operation.
 */
std::error_code GetInsertItemErrorCode(const SessionItem& item, const SessionItem& parent,
                                       const TagIndex& tag_index);

/**
 * @brief Perform validation if item insert is allowed.
 *
 * Will throw InvalidInsertException otherwise.
 */
void ValidateItemInsert(const SessionItem& item, const SessionItem& parent,
                        const TagIndex& tag_index);

/**
 * @brief Returns a error code for potential insert of a new item of the given type.
 */
std::error_code GetInsertTypeErrorCode(const std::string& item_type, const SessionItem& parent,
                                       const TagIndex& tag_index);

/**
 * @brief Perform validation if insert of the new item of the given type insert is allowed.
 *
 * Will throw InvalidInsertException otherwise.
 */
void ValidateItemTypeInsert(const std::string& item_type, const SessionItem& parent,
                            const TagIndex& tag_index);

/**
 * @brief Returns a error code for potential move operation.
 */
std::error_code GetMoveItemErrorCode(const SessionItem& item, const SessionItem& new_parent,
                                     const TagIndex& tag_index);

/**
 * @brief Perform validation if item move is allowed. Will throw InvalidMoveException otherwise.
 */
void ValidateItemMove(const SessionItem& item, const SessionItem& new_parent,
                      const TagIndex& tag_index);

MoveInfo GetMoveOperationInfo(const SessionItem& item, const SessionItem& new_parent,
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
