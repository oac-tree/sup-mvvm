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

#ifndef MVVM_EDITORS_EDITOR_BUILDERS_H_
#define MVVM_EDITORS_EDITOR_BUILDERS_H_

//! @file editor_builders.h
//! Collection of builders for various custom editors

#include <mvvm/editors/editor_types_fwd.h>

namespace mvvm
{

//! Builder for boolean property editor.
MVVM_VIEWMODEL_EXPORT editorbuilder_t BoolEditorBuilder();

//! Builder for ComboProperty editor.
MVVM_VIEWMODEL_EXPORT editorbuilder_t ComboPropertyEditorBuilder();

//! Builder for ComboProperty property editor with selections.
MVVM_VIEWMODEL_EXPORT editorbuilder_t SelectableComboPropertyEditorBuilder();

//! Builder for color property editor.
MVVM_VIEWMODEL_EXPORT editorbuilder_t ColorEditorBuilder();

//! Builder for external property editor.
MVVM_VIEWMODEL_EXPORT editorbuilder_t ExternalPropertyEditorBuilder();

//! Builder for double editor with scientific notation and spinbox functionality.
MVVM_VIEWMODEL_EXPORT editorbuilder_t ScientificSpinBoxEditorBuilder();

//! Builder for integer values editor with possibilities to set limits.
MVVM_VIEWMODEL_EXPORT editorbuilder_t IntegerEditorBuilder();

//! Builder for editor of integer values with possibilities to set limits.
MVVM_VIEWMODEL_EXPORT editorbuilder_t DoubleEditorBuilder();

}  // namespace mvvm

#endif  // MVVM_EDITORS_EDITOR_BUILDERS_H_
