/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_VALIDATEUTILS_H
#define MVVM_MODEL_VALIDATEUTILS_H

//! @file validateutils.h
//! Collection of utility function to check if certain operations on the model are valid.
//! Used to report errors via exception mechanism in an extendable way.

namespace mvvm
{
class SessionItem;
class TagIndex;
}  // namespace mvvm

namespace mvvm::utils
{

//! Converts TagIndex::Append() into actual TagIndex.
TagIndex GetActualInsertTagIndex(SessionItem* parent, const TagIndex& tag_index);

//! Perform validation is item move is allowed. Will throw InvalidMoveException otherwise.
void ValidateItemMove(SessionItem* item, SessionItem* new_parent, const TagIndex& tag_index);

}  // namespace mvvm::utils

#endif  // MVVM_MODEL_VALIDATEUTILS_H
