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

#ifndef MVVM_MODEL_VALIDATE_UTILS_H_
#define MVVM_MODEL_VALIDATE_UTILS_H_

//! @file
//! Collection of utility function to check if certain operations on the model are valid.

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
 * @brief Perform validation if item insert is allowed.
 *
 * @return Success flag, and the reason if insert is not possible.
 */
std::pair<bool, std::string> CanInsertItem(const SessionItem* item, const SessionItem* parent,
                                           const TagIndex& tag_index);

/**
 * @brief Perform validation if item insert is allowed.
 *
 * Will throw InvalidInsertException otherwise.
 */
void ValidateItemInsert(const SessionItem* item, const SessionItem* parent,
                        const TagIndex& tag_index);

/**
 * @brief Perform validation if insert of the new item of the given type is allowed.
 *
 * @return Success flag, and the reason if insert is not possible.
 */
std::pair<bool, std::string> CanInsertType(const std::string& item_type, const SessionItem* parent,
                                           const TagIndex& tag_index);

/**
 * @brief Perform validation if insert of the new item of the given type insert is allowed.
 *
 * Will throw InvalidInsertException otherwise.
 */
void ValidateItemTypeInsert(const std::string& item_type, const SessionItem* parent,
                            const TagIndex& tag_index);

/**
 * @brief Perform validation if item move is allowed.
 *
 * @return Success flag, and the reason if moving is not possible.
 */
std::pair<bool, std::string> CanMoveItem(const SessionItem* item, const SessionItem* new_parent,
                                         const TagIndex& tag_index);

/**
 * @brief Perform validation if item move is allowed. Will throw InvalidMoveException otherwise.
 */
void ValidateItemMove(const SessionItem* item, const SessionItem* new_parent,
                      const TagIndex& tag_index);

/**
 * @brief Perform validation if item take is allowed.
 *
 * @return Success flag, and the reason if take is not possible.
 */
std::pair<bool, std::string> CanTakeItem(const SessionItem* parent, const TagIndex& tag_index);

/**
 * @brief Perform validation if item take is allowed.
 *
 * Will throw InvalidOperationException otherwise.
 */
void ValidateTakeItem(const SessionItem* parent, const TagIndex& tag_index);

}  // namespace mvvm::utils

#endif  // MVVM_MODEL_VALIDATE_UTILS_H_
