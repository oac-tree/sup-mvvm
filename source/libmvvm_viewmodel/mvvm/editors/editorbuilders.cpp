// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/editors/editorbuilders.h"

#include "mvvm/editors/booleditor.h"
#include "mvvm/editors/combopropertyeditor.h"
#include "mvvm/editors/editor_constants.h"
#include "mvvm/model/sessionitem.h"

#include <cmath>

namespace
{
double singleStep(int decimals)
{
  // For item with decimals=3 (i.e. 0.001) single step will be 0.1
  return 1. / std::pow(10., decimals - 1);
}

double getStep(double val)
{
  return val == 0.0 ? 1.0 : val / 100.;
}

}  // namespace

namespace mvvm
{

editorbuilder_t BoolEditorBuilder()
{
  auto builder = [](const SessionItem*) -> editor_t { return std::make_unique<BoolEditor>(); };
  return builder;
}
editorbuilder_t ComboPropertyEditorBuilder()
{
  auto builder = [](const SessionItem*) -> editor_t
  { return std::make_unique<ComboPropertyEditor>(); };
  return builder;
}

}  // namespace mvvm
