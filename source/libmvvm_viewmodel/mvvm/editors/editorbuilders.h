// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

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

}  // namespace mvvm

#endif  // MVVM_EDITORS_EDITORBUILDERS_H
