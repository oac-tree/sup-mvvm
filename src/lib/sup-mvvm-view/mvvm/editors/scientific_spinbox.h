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

#include <QAbstractSpinBox>

namespace mvvm
{

class ScientificSpinBox : public QAbstractSpinBox
{
  Q_OBJECT
  Q_PROPERTY(double value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
  explicit ScientificSpinBox(QWidget* parent_widget = nullptr);
  ~ScientificSpinBox() override;

  ScientificSpinBox(const ScientificSpinBox&) = delete;
  ScientificSpinBox& operator=(const ScientificSpinBox&) = delete;
  ScientificSpinBox(ScientificSpinBox&&) noexcept = delete;
  ScientificSpinBox& operator=(ScientificSpinBox&&) noexcept = delete;

  double value() const;
  void setValue(double val);

  double singleStep() const;
  void setSingleStep(double step);

  double minimum() const;
  void setMinimum(double min);

  double maximum() const;
  void setMaximum(double max);

  void setDecimals(int);
  int decimals() const;

  void stepBy(int steps) override;
  QValidator::State validate(QString&, int&) const override { return QValidator::Acceptable; }
  void fixup(QString&) const override {}

signals:
  void valueChanged(double value);

protected:
  QAbstractSpinBox::StepEnabled stepEnabled() const override;

private:
  void updateValue();
  void updateText();
  bool inRange(double val) const;

  //! Returns the value of the text, the current value if the text is not a valid value in range.
  double valueFromText(const QString& text) const;

  double m_value;
  double m_min;
  double m_max;
  double m_step;
  int m_decimals;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_SCIENTIFIC_SPINBOX_H_
