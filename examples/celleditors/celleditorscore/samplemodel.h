// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef CELLEDITORSCORE_SAMPLEMODEL_H
#define CELLEDITORSCORE_SAMPLEMODEL_H

#include "mvvm/model/applicationmodel.h"
#include "mvvm/model/compounditem.h"

#include <string>

namespace celleditors
{

//! Demo item demonstrating supported properties.

class DemoItem : public mvvm::CompoundItem
{
public:
  static inline const std::string P_BOOL_PROPERTY = "P_BOOL_PROPERTY";
  static inline const std::string P_INTEGER_PROPERTY = "P_INTEGER_PROPERTY";
  static inline const std::string P_STRING_PROPERTY = "P_STRING_PROPERTY";
  static inline const std::string P_DOUBLE_PROPERTY = "P_DOUBLE_PROPERTY";
  static inline const std::string P_COLOR_PROPERTY = "P_COLOR_PROPERTY";
  static inline const std::string P_COMBO_PROPERTY = "P_COMBO_PROPERTY";

  DemoItem();
};

//! Main application model.

class SampleModel : public mvvm::ApplicationModel
{
public:
  SampleModel();
};

}  // namespace celleditors

#endif  // CELLEDITORSCORE_SAMPLEMODEL_H
