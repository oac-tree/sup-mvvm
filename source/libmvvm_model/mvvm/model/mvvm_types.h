// ************************************************************************** //
//
//  Operational Applications UI Foundation
//
// ************************************************************************** //

#ifndef MVVM_MODEL_MVVM_TYPES_H
#define MVVM_MODEL_MVVM_TYPES_H

#include <string>

namespace ModelView
{
class SessionItem;
class SessionModel;

//! Defines constants to be used as SessionItem data role.

namespace ItemDataRole
{
const int IDENTIFIER = 0;  //!< unique identifier
const int DATA = 1;        //!< main data role
const int DISPLAY = 2;     //!< display name
const int APPEARANCE = 3;  //!< appearance flag
const int TOOLTIP = 4;     //!< tooltip for item's data
}  // namespace ItemDataRole

enum Appearance
{
  ENABLED = 1,   // enabled in Qt widgets; when disabled, will be shown in gray
  EDITABLE = 2,  // editable in Qt widgets; readonly otherwise
  VISIBLE = 4    // visible in Qt widgets; doesn't appear in trees and tables otherwise
};

}  // namespace ModelView

#endif  // MVVM_MODEL_MVVM_TYPES_H
