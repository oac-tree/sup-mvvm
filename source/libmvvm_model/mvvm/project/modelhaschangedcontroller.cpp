// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#include "mvvm/project/modelhaschangedcontroller.h"

#include "mvvm/model/tagindex.h"

namespace mvvm
{

//! Constructor of ModelHasChangedController.
//! Acccept 'model' to listen, and a 'callback' to report about changes in a model.

ModelHasChangedController::ModelHasChangedController(ApplicationModel* model, const callback_t &callback)
    : ModelListener(model), m_callback(callback)
{
  SetOnDataChanged([this](auto, auto) { OnChange(); });
  SetOnItemInserted([this](auto, auto) { OnChange(); });
  SetOnItemRemoved([this](auto, auto) { OnChange(); });
  SetOnModelReset([this](auto) { OnChange(); });
}

//! Returns true if the model was changed since last call of resetChanged.

bool ModelHasChangedController::IsChanged() const
{
  return m_has_changed;
}

//! Reset has_changed flag.

void ModelHasChangedController::ResetIsChanged()
{
  m_has_changed = false;
}

//! Sets 'has_changed' flag and reports back to client.

void ModelHasChangedController::OnChange()
{
  m_has_changed = true;
  if (m_callback)
  {
    m_callback();
  }
}

}  // namespace mvvm
