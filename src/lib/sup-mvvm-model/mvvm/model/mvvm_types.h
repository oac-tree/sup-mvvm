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

#ifndef MVVM_MODEL_MVVM_TYPES_H_
#define MVVM_MODEL_MVVM_TYPES_H_

namespace mvvm
{

class SessionItem;
class ISessionModel;

//! Defines constants to be used as SessionItem data role.

namespace DataRole
{
const int kIdentifier = 0;  //!< unique identifier
const int kData = 1;        //!< main data role
const int kDisplay = 2;     //!< display name
const int kAppearance = 3;  //!< appearance flag
const int kTooltip = 4;     //!< tooltip for item's data
const int kEditor = 5;      //!< type of custom editor for the data role
const int kLowerLimit = 6;  //!< lower limit on main data role
const int kUpperLimit = 7;  //!< upper limit on main data role
const int kDataBackup = 8;  //!< secondary data role
const int kTextColor = 9;   //!< text color role
const int kUser = 100;      //!< user role
}  // namespace DataRole

//! Defines bit fields for appearance flag (kAppearance data role).
enum Appearance
{
  kDefault = 0,      //! default appearance (enabled, editable, visible)
  kDisabled = 1,     //! appears as disabled in Qt widgets (visible and editable but shown in gray)
  kReadOnly = 2,     //! readonly in Qt widgets
  kHighlighted = 4,  //! shown differently than the other items
  kHidden = 8,       //! doesn't appear in trees and tables
  kProperty = 16,    //! item appears in property editors; doesn't appear as top-level item
};

}  // namespace mvvm

#endif  // MVVM_MODEL_MVVM_TYPES_H_
