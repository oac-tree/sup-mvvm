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

#ifndef MVVM_EDITORS_EDITORBUILDERS_H
#define MVVM_EDITORS_EDITORBUILDERS_H

#include "mvvm/viewmodel_export.h"

#include <functional>
#include <memory>

namespace mvvm
{

class CustomEditor;
class SessionItem;

//! Collection of methods to build custom editors for trees/tables cells.
//! Used to edit SessionItem data in the context of DefaultEditorFactory.

using editor_t = std::unique_ptr<CustomEditor>;
using editorbuilder_t = std::function<editor_t(const SessionItem*)>;

//! Builder for boolean property editor.
MVVM_VIEWMODEL_EXPORT editorbuilder_t BoolEditorBuilder();

//! Builder for ComboProperty editor.
MVVM_VIEWMODEL_EXPORT editorbuilder_t ComboPropertyEditorBuilder();

//! Builder for color property editor.
MVVM_VIEWMODEL_EXPORT editorbuilder_t ColorEditorBuilder();

}  // namespace mvvm

#endif  // MVVM_EDITORS_EDITORBUILDERS_H
