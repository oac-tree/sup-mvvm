/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_EXTERNAL_PROPERTY_H_
#define MVVM_MODEL_EXTERNAL_PROPERTY_H_

#include <string>

namespace mvvm
{

//! Property to carry text, color and identifier.
//! Can be used to link items with each other.

class ExternalProperty
{
public:
  ExternalProperty();
  ExternalProperty(std::string text, std::string color, std::string identifier = {});

  static ExternalProperty CreateUndefined();

  std::string GetText() const;

  std::string GetColorName() const;

  std::string GetIdentifier() const;

  bool IsEmpty() const;

  std::string ToString() const;
  static ExternalProperty CreateFromString(const std::string& text);

private:
  friend bool operator<(const ExternalProperty& lhs, const ExternalProperty& rhs);
  std::string m_text;
  std::string m_color;
  std::string m_identifier;
};

bool operator==(const ExternalProperty& lhs, const ExternalProperty& rhs);
bool operator!=(const ExternalProperty& lhs, const ExternalProperty& rhs);
bool operator<(const ExternalProperty& lhs, const ExternalProperty& rhs);
bool operator>=(const ExternalProperty& lhs, const ExternalProperty& rhs);

}  // namespace mvvm

#endif  // MVVM_MODEL_EXTERNAL_PROPERTY_H_
