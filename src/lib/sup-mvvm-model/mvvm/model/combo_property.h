/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#ifndef MVVM_MODEL_COMBO_PROPERTY_H_
#define MVVM_MODEL_COMBO_PROPERTY_H_

#include <mvvm/model_export.h>

#include <string>
#include <vector>

namespace mvvm
{

/**
 * @brief The ComboProperty class is a custom property carrying the list of string values with
 * the possibility to select multiple values.
 *
 * Its main usage is to support QComboBox, which contains multiple strings with one string shown at
 * the given moment.
 *
 * Example of combo box with 3 values, and selected_index = 1, selected_value = "B"
 * "A"
 * "B" <-- currently shown in a combo box
 * "C"
 *
 * For multiple selections, SelectableComboEditor should be used.
 * Example of SelectableComboEditor with selected_indices = {0,1}
 * [x] "A"
 * [x] "B'
 * [ ] "C"
 */
class MVVM_MODEL_EXPORT ComboProperty
{
public:
  ComboProperty();
  explicit ComboProperty(std::vector<std::string> values, const std::string& current_value = {});

  static ComboProperty CreateFrom(const std::vector<std::string>& values,
                                  const std::string& current_value = {});

  /**
   * @brief Returns currently selected value.
   *
   * If multiple values are selected, returns the first in the list.
   */
  std::string GetValue() const;

  /**
   * @brief Sets given value as currently selected value.
   *
   * All possible multiple selections will be dropped. The value should be from the list, used for
   * construction.
   */
  void SetValue(const std::string& name);

  /**
   * @brief Returns all possible values.
   */
  std::vector<std::string> GetValues() const;

  /**
   * @brief Sets new values.
   *
   * Current value will be preserved, if it exists in a new list.
   */
  void SetValues(const std::vector<std::string>& values);

  /**
   * @brief Returns list of tool tips for all values.
   */
  std::vector<std::string> GetToolTips() const;

  /**
   * @brief Sets tool tips for all values.
   *
   * Method doesn't validate the number of tooltips provided. Combo property editor will most-likely
   * ignore extra tooltips, if exist.
   */
  void SetToolTips(const std::vector<std::string>& tooltips);

  /**
   * @brief Returns index of the currently selected item.
   */
  int GetCurrentIndex() const;

  /**
   * @brief Sets given index as current selection.
   *
   * All multiple selections will be dropped, if exist.
   */
  void SetCurrentIndex(int index);

  /**
   * @brief Returns list of currently selected indices.
   */
  std::vector<int> GetSelectedIndices() const;

  /**
   * @brief Sets given list as currently selected indices.
   */
  void SetSelectedIndices(const std::vector<int>& selected_indices);

  /**
   * @brief Returns list of currently selected values.
   */
  std::vector<std::string> GetSelectedValues() const;

  /**
   * @brief Sets selection flag for given index.
   *
   * If false, the index will be excluded from selection.
   */
  void SetSelected(int index, bool value = true);

  /**
   * @brief Set selection flag for given value.
   *
   * The value should be from the list of values used for construction.
   */
  void SetSelected(const std::string& name, bool value = true);

  /**
   * @brief Returns a label representing current selection status.
   */
  std::string GetLabel() const;

  ComboProperty& operator<<(const std::string& str);
  ComboProperty& operator<<(const std::vector<std::string>& str);
  bool operator==(const ComboProperty& other) const;
  bool operator!=(const ComboProperty& other) const;
  bool operator<(const ComboProperty& other) const;
  bool operator>=(const ComboProperty& other) const;

  // serialization related methods (FIXME they do not belong to this place)

  /**
   * @brief Returns a single string containing values delimited with ';'.
   */
  std::string GetStringOfValues() const;

  /**
   * @brief Sets values from the string containing delimeter ';'.
   */
  void SetValuesFromString(const std::string& values);

  /**
   * @brief Returns a string with coma separated list of selected indices.
   */
  std::string GetStringOfSelections() const;

  /**
   * @brief Sets selection from its srting representation.
   */
  void SetSelectionFromString(const std::string& values);

private:
  std::vector<std::string> m_values;
  std::vector<std::string> m_tooltips;
  std::vector<int> m_selected_indices;
};

}  // namespace mvvm

#endif  // MVVM_MODEL_COMBO_PROPERTY_H_
