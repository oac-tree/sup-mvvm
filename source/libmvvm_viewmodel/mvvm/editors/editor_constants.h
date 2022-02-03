// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_EDITORS_EDITOR_CONSTANTS_H
#define MVVM_EDITORS_EDITOR_CONSTANTS_H

//! @file editor_constants.h
//! Collection of constants specific for cell editing.

#include <string>

namespace mvvm::constants
{

const std::string BoolEditorType = "BoolEditor";
const std::string ComboPropertyEditorType = "ComboPropertyEditor";

const int default_double_decimals = 4;  //! number of digits after decimal points

}  // namespace mvvm::constants

#endif  // MVVM_EDITORS_EDITOR_CONSTANTS_H
