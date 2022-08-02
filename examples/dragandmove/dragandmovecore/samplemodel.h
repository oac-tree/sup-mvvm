// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef DRAGANDMOVECORE_SAMPLEMODEL_H
#define DRAGANDMOVECORE_SAMPLEMODEL_H

#include <mvvm/model/application_model.h>

namespace mvvm
{
class SessionItem;
}

namespace DragAndMove
{

//! Main application model.

class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();

  void AppendRandomItem(mvvm::SessionItem* container);

private:
  void PopulateModel();
};

}  // namespace DragAndMove

#endif  // DRAGANDMOVECORE_SAMPLEMODEL_H
