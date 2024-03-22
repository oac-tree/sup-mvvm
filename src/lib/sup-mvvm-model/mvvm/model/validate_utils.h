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

#ifndef MVVM_MODEL_VALIDATE_UTILS_H_
#define MVVM_MODEL_VALIDATE_UTILS_H_

/**
 * Collection of utility function to check if certain operations on the model are valid.
 */

#include <optional>
#include <string>

namespace mvvm
{
class SessionModelInterface;
class SessionItem;
class TagIndex;
}  // namespace mvvm

namespace mvvm::utils
{

/**
 * @brief Converts TagIndex::Append() into actual TagIndex.
 */
TagIndex GetActualInsertTagIndex(const SessionItem* parent, const TagIndex& tag_index);

/**
 * @brief Perform validation if item insert is allowed.
 *
 * @return Success flag, and the reason if insert is not possible.
 */
std::pair<bool, std::string> CanInsertItem(const SessionItem* item, const SessionItem* parent,
                                           const TagIndex& tag_index);

/**
 * @brief Perform validation if item insert is allowed. Will throw InvalidInsertException otherwise.
 */
void ValidateItemInsert(const SessionItem* item, const SessionItem* parent,
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
std::pair<bool, std::string> CanTakeItem(const SessionModelInterface* model,
                                         const SessionItem* parent, const TagIndex& tag_index);

/**
 * @brief Perform validation if item take is allowed. Will throw InvalidOperationException
 * otherwise.
 */
void ValidateTakeItem(const SessionModelInterface* model, const SessionItem* parent,
                      const TagIndex& tag_index);

}  // namespace mvvm::utils

#endif  // MVVM_MODEL_VALIDATE_UTILS_H_
