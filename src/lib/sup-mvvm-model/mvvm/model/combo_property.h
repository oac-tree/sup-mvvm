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

#ifndef MVVM_MODEL_COMBO_PROPERTY_H_
#define MVVM_MODEL_COMBO_PROPERTY_H_

#include <cstdint>
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
class ComboProperty
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
  std::int32_t GetCurrentIndex() const;

  /**
   * @brief Sets given index as current selection.
   *
   * All multiple selections will be dropped, if exist.
   */
  void SetCurrentIndex(std::int32_t index);

  /**
   * @brief Returns list of currently selected indices.
   */
  std::vector<std::int32_t> GetSelectedIndices() const;

  /**
   * @brief Sets given list as currently selected indices.
   */
  void SetSelectedIndices(const std::vector<std::int32_t>& selected_indices);

  /**
   * @brief Returns list of currently selected values.
   */
  std::vector<std::string> GetSelectedValues() const;

  /**
   * @brief Sets selection flag for given index.
   *
   * If false, the index will be excluded from selection.
   */
  void SetSelected(std::int32_t index, bool value = true);

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

  /**
   * @brief Appends a value.
   */
  ComboProperty& operator<<(const std::string& str) &;

  /**
   * @brief Appends a value to a temporary and returns it by value.
   */
  ComboProperty operator<<(const std::string& str) &&;

  /**
   * @brief Appends a list of values.
   */
  ComboProperty& operator<<(const std::vector<std::string>& str) &;

  /**
   * @brief Appends a list of values to a temporary and returns it by value.
   */
  ComboProperty operator<<(const std::vector<std::string>& str) &&;

  /**
   * @brief Return a string representing this ComboProperty.
   *
   * This function is used for XML serialization and debugging.
   */
  std::string ToString() const;

  /**
   * @brief Create combo property from its string representation.
   *
   * This function is used for XML serialization and debugging.
   *
   * String "values[a1;a2;a3] selection[1,2]" will create equivalent of
   * Combo combo({"a1","a2","a3"});
   * combo.SetSelected({1, 2});
   */
  static ComboProperty CreateFromString(const std::string& str);

private:
  friend bool operator<(const ComboProperty& lhs, const ComboProperty& rhs);
  std::vector<std::string> m_values;
  std::vector<std::string> m_tooltips;
  std::vector<std::int32_t> m_selected_indices;
};

bool operator==(const ComboProperty& lhs, const ComboProperty& rhs);
bool operator!=(const ComboProperty& lhs, const ComboProperty& rhs);
bool operator<(const ComboProperty& lhs, const ComboProperty& rhs);
bool operator>=(const ComboProperty& lhs, const ComboProperty& rhs);

}  // namespace mvvm


#endif  // MVVM_MODEL_COMBO_PROPERTY_H_
