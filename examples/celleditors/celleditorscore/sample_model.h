// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef CELLEDITORS_CELLEDITORSCORE_SAMPLE_MODEL_H_
#define CELLEDITORS_CELLEDITORSCORE_SAMPLE_MODEL_H_

#include "mvvm/model/application_model.h"
#include "mvvm/model/compound_item.h"

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

#endif  // CELLEDITORS_CELLEDITORSCORE_SAMPLE_MODEL_H_
