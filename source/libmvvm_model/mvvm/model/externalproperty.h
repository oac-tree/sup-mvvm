// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_MODEL_EXTERNALPROPERTY_H
#define MVVM_MODEL_EXTERNALPROPERTY_H

#include "mvvm/core/variant.h"
#include "mvvm/model_export.h"

#include <string>

namespace mvvm
{

//! Property to carry text, color and identifier.
//! Can be used to link items with each other.

class MVVM_MODEL_EXPORT ExternalProperty
{
public:
  ExternalProperty();
  ExternalProperty(std::string text, std::string color, std::string identifier = {});

  static ExternalProperty undefined();

  std::string text() const;

  std::string color() const;

  std::string identifier() const;

  bool isValid() const;

  bool operator==(const ExternalProperty& other) const;
  bool operator!=(const ExternalProperty& other) const;
  bool operator<(const ExternalProperty& other) const;

private:
  std::string m_text;
  std::string m_color;
  std::string m_identifier;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_EXTERNALPROPERTY_H
