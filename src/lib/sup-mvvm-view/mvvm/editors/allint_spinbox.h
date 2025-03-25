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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_EDITORS_ALLINT_SPINBOX_H_
#define MVVM_EDITORS_ALLINT_SPINBOX_H_

#include <mvvm/viewmodel_export.h>

#include <QAbstractSpinBox>

namespace mvvm
{

class ILimitedInteger;

/**
 * @brief The AllIntSpinBox class is intended for editing all integer-like types: int8, uin8,
 * int16, uin16, int32, uint32, int64 and uint64.
 *
 * @details Current behavior: when user enters manually long integer which is above current limits,
 * and pushes enter, the value is returning to initial value. Desired behavior: the value should be
 * set to allowed maximum. Requires fix in LimitedInteger::SetValueFromText.
 *
 * @details Current behavior: when user changes the value in the text editor and then switches to
 * another window (focus out event), no signals are emitted, and the value returns to initial value.
 * QSpinBox behaves itself differently and set last entered value.
 *
 * @details Current behavior: when the user increases or decreases the value with spin up/down
 * buttons, no signals are emitted until he presses the enter button. That's Ok since QSpinBox
 * behaves itself by default the same.
 */

class MVVM_VIEWMODEL_EXPORT AllIntSpinBox : public QAbstractSpinBox
{
  Q_OBJECT

  Q_PROPERTY(QVariant value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
  explicit AllIntSpinBox(QWidget *parent = nullptr);
  ~AllIntSpinBox() override;

  void SetLimitedInteger(std::unique_ptr<ILimitedInteger> value);

  QVariant value() const;

  QValidator::State validate(QString &input, int &pos) const override;

  void fixup(QString &input) const override;

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
  void setValue(const QVariant &value);

signals:
  void valueChanged(QVariant);

private:
  void OnEditingFinished();
  void UpdateTextField();

  std::unique_ptr<ILimitedInteger> m_value;
  //!< flag to prevent notifications in the course of value change, but before "editingFinished"
  //!< event
  bool m_cached_value_was_changed{false};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_ALLINT_SPINBOX_H_
