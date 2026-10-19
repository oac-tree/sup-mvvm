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

#ifndef MVVM_EDITORS_ALLINT_SPINBOX_H_
#define MVVM_EDITORS_ALLINT_SPINBOX_H_

#include <mvvm/core/variant.h>

#include <QAbstractSpinBox>

namespace mvvm
{

class ILimitedInteger;

/**
 * @brief The AllIntSpinBox class is intended for editing all integer-like types: int8, uint8,
 * int16, uin16, int32, uint32, int64 and uint64.
 *
 * @details Current behavior: when user enters manually long integer which is above current limits,
 * and pushes enter, the value is returning to initial value. Desired behavior: the value should be
 * set to allowed maximum. Requires fix in LimitedInteger::SetValueFromText.
 *
 * @details The value property reports the typed text before editingFinished. The cell delegate
 * reads it on focus-out before the spin box processes the event.
 *
 * @details Current behavior: when the user increases or decreases the value with spin up/down
 * buttons, no signals are emitted until he presses the enter button. That's Ok since QSpinBox
 * behaves itself by default the same.
 */

class AllIntSpinBox : public QAbstractSpinBox
{
  Q_OBJECT

  Q_PROPERTY(QVariant value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
  explicit AllIntSpinBox(QWidget* parent = nullptr);
  ~AllIntSpinBox() override;

  AllIntSpinBox(const AllIntSpinBox&) = delete;
  AllIntSpinBox& operator=(const AllIntSpinBox&) = delete;
  AllIntSpinBox(AllIntSpinBox&&) noexcept = delete;
  AllIntSpinBox& operator=(AllIntSpinBox&&) noexcept = delete;

  void SetLimitedInteger(std::unique_ptr<ILimitedInteger> value);

  /**
   * @brief Sets limits for the limited integer created by the first setValue call.
   *
   * @details Default-constructed limits mean the limits of the integer type.
   */
  void SetRange(const variant_t& lower_limit, const variant_t& upper_limit);

  QVariant value() const;

  QValidator::State validate(QString& input, int& pos) const override;

  void fixup(QString& input) const override;

  void stepBy(int steps) override;

  /**
   * @brief Provide check if cached value has been changed and notify valueChanged if it was the
   * case.
   */
  void CheckNotify();

  /**
   * @brief Returns underlying limited integer.
   */
  ILimitedInteger* GetLimitedInteger();

protected:
  QAbstractSpinBox::StepEnabled stepEnabled() const override;

public slots:
  void setValue(const QVariant& value);

signals:
  void valueChanged(QVariant);

private:
  void OnEditingFinished();

  //! Returns the value parsed from the text, or an invalid variant if the text has no new valid
  //! value.
  variant_t GetTextValue() const;
  void UpdateTextField();

  std::unique_ptr<ILimitedInteger> m_value;
  variant_t m_lower_limit;
  variant_t m_upper_limit;
  //!< flag to prevent notifications in the course of value change, but before "editingFinished"
  //!< event
  bool m_cached_value_was_changed{false};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_ALLINT_SPINBOX_H_
