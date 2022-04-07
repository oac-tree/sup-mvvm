// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef CELLEDITORS_CELLEDITORSCORE_SAMPLEMODEL_H
#define CELLEDITORS_CELLEDITORSCORE_SAMPLEMODEL_H

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/compounditem.h"

#include <string>

namespace celleditors
{

//! Demo item demonstrating supported properties.

class DemoItem : public mvvm::CompoundItem
{
public:
  DemoItem();
};

//! Main application model.

class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();
};

}  // namespace celleditors

#endif  // CELLEDITORS_CELLEDITORSCORE_SAMPLEMODEL_H
