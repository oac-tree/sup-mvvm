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

namespace DataRole
{
const int kIdentifier = 0;  //!< unique identifier
const int kData = 1;        //!< main data role
const int kDisplay = 2;     //!< display name
const int kAppearance = 3;  //!< appearance flag
const int kTooltip = 4;     //!< tooltip for item's data
}  // namespace DataRole

enum Appearance
{
  kEnabled = 1,   // enabled in Qt widgets; when disabled, will be shown in gray
  kEditable = 2,  // editable in Qt widgets; readonly otherwise
  kVisible = 4    // visible in Qt widgets; doesn't appear in trees and tables otherwise
};

}  // namespace ModelView

#endif  // MVVM_MODEL_MVVM_TYPES_H
