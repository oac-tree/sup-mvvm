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

#ifndef MVVM_STANDARDITEMS_EDITOR_CONSTANTS_H_
#define MVVM_STANDARDITEMS_EDITOR_CONSTANTS_H_

//! @file
//! Collection of constants specific for cell editing.

#include <cstdint>
#include <string_view>

namespace mvvm::constants
{

constexpr std::string_view kBoolEditorType = "BoolEditor";
constexpr std::string_view kComboPropertyEditorType = "ComboPropertyEditor";
constexpr std::string_view kSelectableComboPropertyEditorType = "SelectableComboPropertyEditor";
constexpr std::string_view kColorEditorType = "ColorEditor";
constexpr std::string_view kExternalPropertyEditorType = "ExternalPropertyEditor";
constexpr std::string_view kScientificSpinboxEditorType = "ScientificSpinboxEditor";
constexpr std::string_view kIntegerEditorType = "IntegerEditor";
constexpr std::string_view kDoubleEditorType = "DoubleEditor";
constexpr std::string_view kAllIntSpinBoxEditorType = "AllIntSpinBoxEditor";
constexpr std::string_view kFloatSpinBoxEditorType = "kFloatSpinBoxEditorType";
constexpr std::string_view kStringCompleterEditorType = "kStringCompleterEditorType";
constexpr std::string_view kComboCompleterEditorType = "kComboCompleterEditorType";

const std::int32_t kDefaultDoubleDecimals = 4;  //! number of digits after decimal points

}  // namespace mvvm::constants

#endif  // MVVM_STANDARDITEMS_EDITOR_CONSTANTS_H_
