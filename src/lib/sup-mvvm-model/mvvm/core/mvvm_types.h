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

#ifndef MVVM_MODEL_MVVM_TYPES_H_
#define MVVM_MODEL_MVVM_TYPES_H_

#include <cstdint>

namespace mvvm
{

using role_t = std::int32_t;

//! Defines constants to be used as SessionItem data role.
namespace DataRole
{
const role_t kIdentifier = 0;  //!< unique identifier
const role_t kData = 1;        //!< main data role
const role_t kDisplay = 2;     //!< display name
const role_t kAppearance = 3;  //!< appearance flag
const role_t kTooltip = 4;     //!< tooltip for item's data
const role_t kEditor = 5;      //!< type of custom editor for the data role
const role_t kLowerLimit = 6;  //!< lower limit on main data role
const role_t kUpperLimit = 7;  //!< upper limit on main data role
const role_t kDataBackup = 8;  //!< secondary data role
const role_t kTextColor = 9;   //!< text color role
const role_t kUser = 100;      //!< user role
}  // namespace DataRole

/**
 * @brief The Appearance enum defines bit fields for various item appearance states.
 *
 * A combination of Appearance fields is stored on the board of SessionItem as the kAppearance data
 * role. By default, the item doesn't have any special Appearance fields defined, i.e., the item is
 * enabled, editable, visible, etc.
 *
 * The Appearance enum is intended to work together with the convenience Flags<> class and has
 * obligatory last member named "size".
 */
enum Appearance : std::uint8_t
{
  kDisabled,     //! appears as disabled in Qt widgets (visible and editable but shown in gray)
  kReadOnly,     //! readonly in Qt widgets
  kHighlighted,  //! shown differently than the other items
  kHidden,       //! doesn't appear in trees and tables
  kProperty,     //! item appears in property editors; doesn't appear as top-level item
  kEditableDisplayName,  //! display name of the item can be edited
  size                   //! obligatory name
};

}  // namespace mvvm

#endif  // MVVM_MODEL_MVVM_TYPES_H_
