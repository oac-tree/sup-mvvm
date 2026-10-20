/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef MVVM_EDITORS_EDITOR_BUILDERS_H_
#define MVVM_EDITORS_EDITOR_BUILDERS_H_

//! @file editor_builders.h
//! Collection of builders for various custom editors

#include <mvvm/providers/editor_types_fwd.h>

namespace mvvm
{

/**
 * @brief Returns builder function to create editors for bool based variants.
 *
 * @details An item with Appearance::kNullable gets NullableBoolEditor, a combo with the
 * placeholder text from DataRole::kPlaceholder, "True" and "False".
 */
editorbuilder_t BoolEditorBuilder();

/**
 * @brief Returns builder function to create editors for ComboProperty based variants.
 */
editorbuilder_t ComboPropertyEditorBuilder();

/**
 * @brief Returns builder function to create editors for ComboProperty based variants.
 *
 * @details It has a fancy feature with possibility to make multiple selections.
 */
editorbuilder_t SelectableComboPropertyEditorBuilder();

/**
 * @brief Returns builder function to create color editors.
 */
editorbuilder_t ColorEditorBuilder();

/**
 * @brief Returns builder function to create ExternalPropertyEditors editors.
 */
editorbuilder_t ExternalPropertyEditorBuilder();

/**
 * @brief Returns builder function to create ExternalPropertyEditors editors.
 */

/**
 * @brief Returns builder function to create editors for float32 and float64 based variants.
 *
 * @details It relies on ScientificSpinBox. Limits come from the item, precision from
 * DataRole::kPrecision, notation from Appearance::kScientific.
 *
 * @details An item with Appearance::kNullable gets an editor accepting empty input. The type and
 * the start value come from DataRole::kDefaultValue, the placeholder text from
 * DataRole::kPlaceholder.
 */

editorbuilder_t FloatEditorBuilder();

/**
 * @brief Returns builder function to create editors for int64 based variants.
 *
 * @details About to become obsolete in the favor of AllIntSpinBoxEditorBuilder()
 */
editorbuilder_t LongIntSpinBoxEditorBuilder();

/**
 * @brief Returns builder function to create editors for integer based variants.
 *
 * @details It relies on QSpinBox with additional option to set limits from SessionItem. Used only
 * for constants::kIntegerEditorType, to compare with AllIntSpinBox.
 */
editorbuilder_t IntegerEditorBuilder();

/**
 * @brief Returns builder function to create editors for all integer-like QVariants.
 *
 * @details It relies on AllIntSpinBox which knows how to work with all integer-like types:
 * char8, int8, uint8, int16, uin16, int32, uin32, int64, uin64.
 *
 * @details An item with Appearance::kNullable gets an editor accepting empty input. The type and
 * the start value come from DataRole::kDefaultValue, the placeholder text from
 * DataRole::kPlaceholder.
 *
 */
editorbuilder_t AllIntSpinBoxEditorBuilder();

/**
 * @brief Returns builder function to create editors for double based variants.
 *
 * @details It relies on QDoubleSpinBox with additional option to set limits from SessionItem.
 *
 * @details About to become obsolete in the favor of FloatEditorBuilder()
 */
editorbuilder_t DoubleEditorBuilder();

}  // namespace mvvm

#endif  // MVVM_EDITORS_EDITOR_BUILDERS_H_
