/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_EXTERNAL_PROPERTY_H_
#define MVVM_MODEL_EXTERNAL_PROPERTY_H_

#include <mvvm/model_export.h>

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

  static ExternalProperty CreateUndefined();

  std::string GetText() const;

  std::string GetColorName() const;

  std::string GetIdentifier() const;

  bool IsEmpty() const;

  bool operator==(const ExternalProperty& other) const;
  bool operator!=(const ExternalProperty& other) const;
  bool operator<(const ExternalProperty& other) const;

  std::string ToString() const;
  static ExternalProperty CreateFromString(const std::string& text);

private:
  std::string m_text;
  std::string m_color;
  std::string m_identifier;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_EXTERNAL_PROPERTY_H_
