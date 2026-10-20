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

#ifndef MVVM_EDITORS_SCIENTIFIC_SPINBOX_H_
#define MVVM_EDITORS_SCIENTIFIC_SPINBOX_H_

#include <mvvm/core/variant.h>

#include <QAbstractSpinBox>
#include <memory>

namespace mvvm
{

class ILimitedFloat;

/**
 * @brief The ScientificSpinBox class is intended for editing float32 and float64 values.
 *
 * @details The value property is a QVariant of the type of the edited value, float32 stays float32.
 * The text is in fixed or exponential notation with the given precision. Fixed and exponential
 * notation are accepted as input.
 *
 * @details The value is stored exactly, the precision affects the text only. The text of the
 * current value leaves the value unchanged.
 *
 * @details The value property reports the typed text before editingFinished. The cell delegate
 * reads it on focus-out before the spin box processes the event.
 *
 * @details With a nullable limited float, empty text means no value. The value property is then an
 * invalid QVariant. The first step from no value sets the default value.
 *
 * @details No signals are emitted on steps until editing is finished, as for QSpinBox.
 */
class ScientificSpinBox : public QAbstractSpinBox
{
  Q_OBJECT

  Q_PROPERTY(QVariant value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
  explicit ScientificSpinBox(QWidget* parent_widget = nullptr);
  ~ScientificSpinBox() override;

  ScientificSpinBox(const ScientificSpinBox&) = delete;
  ScientificSpinBox& operator=(const ScientificSpinBox&) = delete;
  ScientificSpinBox(ScientificSpinBox&&) noexcept = delete;
  ScientificSpinBox& operator=(ScientificSpinBox&&) noexcept = delete;

  /**
   * @brief Sets the limited float to edit.
   *
   * @details The single step of the editor is applied to it.
   */
  void SetLimitedFloat(std::unique_ptr<ILimitedFloat> value);

  /**
   * @brief Returns underlying limited float.
   */
  ILimitedFloat* GetLimitedFloat();

  /**
   * @brief Sets the text shown when the field is empty.
   */
  void SetPlaceholderText(const QString& text);

  /**
   * @brief Returns the text shown when the field is empty.
   */
  QString GetPlaceholderText() const;

  /**
   * @brief Sets limits for the limited float created by the first setValue call.
   *
   * @details Default-constructed limits mean the limits of the float type.
   */
  void SetRange(const variant_t& lower_limit, const variant_t& upper_limit);

  /**
   * @brief Sets the number of digits after decimal point in the text.
   */
  void SetPrecision(std::int32_t precision);

  /**
   * @brief Returns the number of digits after decimal point in the text.
   */
  std::int32_t GetPrecision() const;

  /**
   * @brief Sets exponential notation of the text, fixed notation otherwise.
   */
  void SetScientific(bool value);

  /**
   * @brief Returns true if the text is in exponential notation.
   */
  bool IsScientific() const;

  /**
   * @brief Sets the step of the up and down buttons.
   */
  void SetSingleStep(double step);

  /**
   * @brief Returns the step of the up and down buttons.
   */
  double GetSingleStep() const;

  QVariant value() const;

  QValidator::State validate(QString& input, int& pos) const override;

  void fixup(QString& input) const override;

  void stepBy(int steps) override;

  /**
   * @brief Provide check if cached value has been changed and notify valueChanged if it was the
   * case.
   */
  void CheckNotify();

protected:
  QAbstractSpinBox::StepEnabled stepEnabled() const override;

public slots:
  void setValue(const QVariant& value);

signals:
  void valueChanged(QVariant);

private:
  void OnEditingFinished();
  void UpdateTextField();

  //! Returns the text of the current value.
  std::string GetValueText() const;

  std::unique_ptr<ILimitedFloat> m_value;
  variant_t m_lower_limit;
  variant_t m_upper_limit;
  std::int32_t m_precision{0};
  bool m_scientific{false};
  double m_single_step{1.0};
  //!< flag to prevent notifications in the course of value change, but before "editingFinished"
  //!< event
  bool m_cached_value_was_changed{false};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_SCIENTIFIC_SPINBOX_H_
