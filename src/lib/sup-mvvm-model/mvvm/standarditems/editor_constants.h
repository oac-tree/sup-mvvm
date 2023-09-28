/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_STANDARDITEMS_EDITOR_CONSTANTS_H_
#define MVVM_STANDARDITEMS_EDITOR_CONSTANTS_H_

//! @file
//! Collection of constants specific for cell editing.

#include <string>

namespace mvvm::constants
{

const std::string kBoolEditorType = "BoolEditor";
const std::string kComboPropertyEditorType = "ComboPropertyEditor";
const std::string kSelectableComboPropertyEditorType = "SelectableComboPropertyEditor";
const std::string kColorEditorType = "ColorEditor";
const std::string kExternalPropertyEditorType = "ExternalPropertyEditor";
const std::string kScientificSpinboxEditorType = "ScientificSpinboxEditor";
const std::string kLongIntSpinBoxEditorType = "LongIntSpinBoxEditor";
const std::string kIntegerEditorType = "IntegerEditor";
const std::string kDoubleEditorType = "DoubleEditor";
const std::string kAllIntSpinBoxEditorType = "AllIntSpinBoxEditor";
const std::string kFloatSpinBoxEditorType = "kFloatSpinBoxEditorType";

const int kDefaultDoubleDecimals = 4;  //! number of digits after decimal points

}  // namespace mvvm::constants

#endif  // MVVM_STANDARDITEMS_EDITOR_CONSTANTS_H_
