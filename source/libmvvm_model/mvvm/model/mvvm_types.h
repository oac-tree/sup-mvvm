/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_MVVM_TYPES_H
#define MVVM_MODEL_MVVM_TYPES_H

#include <string>

namespace mvvm
{
class SessionItem;
class SessionModel;

//! Defines constants to be used as SessionItem data role.

namespace DataRole
{
const int kIdentifier = 0;  //!< unique identifier
const int kData = 1;        //!< main data role
const int kDisplay = 2;     //!< display name
const int kAppearance = 3;  //!< appearance flag
const int kTooltip = 4;     //!< tooltip for item's data
}  // namespace DataRole

//! Defines bit fields for appearance flag (kAppearance data role).
enum Appearance
{
  kEnabled = 1,   // enabled in Qt widgets; when disabled, will be shown in gray
  kEditable = 2,  // editable in Qt widgets; readonly otherwise
  kVisible = 4    // visible in Qt widgets; doesn't appear in trees and tables otherwise
};

}  // namespace mvvm

#endif  // MVVM_MODEL_MVVM_TYPES_H
