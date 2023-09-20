/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_COMBO_PROPERTY_H_
#define MVVM_MODEL_COMBO_PROPERTY_H_

#include <mvvm/model_export.h>

#include <string>
#include <vector>

namespace mvvm
{

//! Custom property to define list of string values with multiple selections.

class MVVM_MODEL_EXPORT ComboProperty
{
public:
  ComboProperty();
  explicit ComboProperty(std::vector<std::string> values, const std::string& current_value = {});

  static ComboProperty CreateFrom(const std::vector<std::string>& values,
                                  const std::string& current_value = {});

  std::string GetValue() const;
  void SetValue(const std::string& name);

  std::vector<std::string> GetValues() const;
  void SetValues(const std::vector<std::string>& values);

  std::vector<std::string> GetToolTips() const;
  void SetToolTips(const std::vector<std::string>& tooltips);

  int GetCurrentIndex() const;
  void SetCurrentIndex(int index);

  ComboProperty& operator<<(const std::string& str);
  ComboProperty& operator<<(const std::vector<std::string>& str);
  bool operator==(const ComboProperty& other) const;
  bool operator!=(const ComboProperty& other) const;
  bool operator<(const ComboProperty& other) const;
  bool operator>=(const ComboProperty& other) const;

  std::string GetStringOfValues() const;
  void SetStringOfValues(const std::string& values);

  std::vector<int> GetSelectedIndices() const;
  std::vector<std::string> SetSelectedValues() const;

  void SetSelected(int index, bool value = true);
  void SetSelected(const std::string& name, bool value = true);

  std::string GetStringOfSelections() const;
  void SetStringOfSelections(const std::string& values);

  std::string GetLabel() const;

private:
  std::vector<std::string> m_values;
  std::vector<std::string> m_tooltips;
  std::vector<int> m_selected_indices;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_COMBO_PROPERTY_H_
