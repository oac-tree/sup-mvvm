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

#ifndef MVVM_EDITORS_EDITOR_BUILDERS_H_
#define MVVM_EDITORS_EDITOR_BUILDERS_H_

//! @file editor_builders.h
//! Collection of builders for various custom editors

#include <mvvm/editors/editor_types_fwd.h>

namespace mvvm
{

/**
 * @brief Returns builder function to create editors for bool based variants.
 */
MVVM_VIEWMODEL_EXPORT editorbuilder_t BoolEditorBuilder();

/**
 * @brief Returns builder function to create editors for ComboProperty based variants.
 */
MVVM_VIEWMODEL_EXPORT editorbuilder_t ComboPropertyEditorBuilder();

/**
 * @brief Returns builder function to create editors for ComboProperty based variants.
 *
 * @details It has a fancy feature with possibility to make multiple selections.
 */
MVVM_VIEWMODEL_EXPORT editorbuilder_t SelectableComboPropertyEditorBuilder();

/**
 * @brief Returns builder function to create color editors.
 */
MVVM_VIEWMODEL_EXPORT editorbuilder_t ColorEditorBuilder();

/**
 * @brief Returns builder function to create ExternalPropertyEditors editors.
 */
MVVM_VIEWMODEL_EXPORT editorbuilder_t ExternalPropertyEditorBuilder();

/**
 * @brief Returns builder function to create ExternalPropertyEditors editors.
 */

/**
 * @brief Returns builder function to create editors for double based variants.
 *
 * @details It relies on ScientificSpinBox with additional option to edit text with scientific
 * notations 10e-12.
 *
 * @details About to be integrated to FloatEditorBuilder().
 */

MVVM_VIEWMODEL_EXPORT editorbuilder_t ScientificSpinBoxEditorBuilder();

/**
 * @brief Returns builder function to create editors for int64 based variants.
 *
 * @details About to become obsolete in the favor of AllIntSpinBoxEditorBuilder()
 */
MVVM_VIEWMODEL_EXPORT editorbuilder_t LongIntSpinBoxEditorBuilder();

/**
 * @brief Returns builder function to create editors for integer based variants.
 *
 * @details It relies on QSpinBox with additional option to set limits from SessionItem.
 *
 * @details About to become obsolete in the favor of AllIntSpinBoxEditorBuilder()
 */
MVVM_VIEWMODEL_EXPORT editorbuilder_t IntegerEditorBuilder();

/**
 * @brief Returns builder function to create editors for all integer-like QVariants.
 *
 * @details It relies on AllIntSpinBoxEditor which knows how to work with all integer-like types:
 * int8, uint8, int16, uin16, int32, uin32, int64, uin64.
 *
 * @details This builder will eventually replace IntegerEditorBuilder.
 */
MVVM_VIEWMODEL_EXPORT editorbuilder_t AllIntSpinBoxEditorBuilder();

/**
 * @brief Returns builder function to create editors for double based variants.
 *
 * @details It relies on QDoubleSpinBox with additional option to set limits from SessionItem.
 *
 * @details About to become obsolete in the favor of FloatEditorBuilder()
 */
MVVM_VIEWMODEL_EXPORT editorbuilder_t DoubleEditorBuilder();

/**
 * @brief Returns builder function to create editors for float32 and float64 based variants.
 *
 * @details It relies on FloatSpinBox which knows how to work with both QVariant(float32) and
 * QVariant(float64).
 *
 * @details This builder will eventually replace DoubleEditorBuilder.
 */
MVVM_VIEWMODEL_EXPORT editorbuilder_t FloatEditorBuilder();

}  // namespace mvvm

#endif  // MVVM_EDITORS_EDITOR_BUILDERS_H_
