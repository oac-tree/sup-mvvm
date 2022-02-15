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

#ifndef MVVM_EDITORS_EDITOR_CONSTANTS_H
#define MVVM_EDITORS_EDITOR_CONSTANTS_H

//! @file editor_constants.h
//! Collection of constants specific for cell editing.

#include <string>

namespace mvvm::constants
{

const std::string BoolEditorType = "BoolEditor";
const std::string ComboPropertyEditorType = "ComboPropertyEditor";
const std::string COlorEditorType = "ColorEditor";

const int default_double_decimals = 4;  //! number of digits after decimal points

}  // namespace mvvm::constants

#endif  // MVVM_EDITORS_EDITOR_CONSTANTS_H
