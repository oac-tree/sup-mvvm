/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Rafael Silva (EXT)
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

#ifndef MVVM_EDITORS_LONGINT_SPINBOX_H_
#define MVVM_EDITORS_LONGINT_SPINBOX_H_

#include <mvvm/viewmodel_export.h>

#include <QAbstractSpinBox>

namespace mvvm
{

//! Custom spin box widget for long integers (64 bits in size).
class MVVM_VIEWMODEL_EXPORT LongIntSpinBox : public QAbstractSpinBox
{
  Q_OBJECT

  Q_PROPERTY(qint64 value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged USER true)
  Q_PROPERTY(qint64 singleStep MEMBER m_step READ singleStep WRITE setSingleStep)
  Q_PROPERTY(qint64 minimum MEMBER m_minimum READ minimum WRITE setMinimum)
  Q_PROPERTY(qint64 maximum MEMBER m_maximum READ maximum WRITE setMaximum)

public:
  explicit LongIntSpinBox(QWidget *parent = nullptr);
  ~LongIntSpinBox() override;

  qint64 value() const;

  qint64 singleStep() const;
  void setSingleStep(qint64 step_value);

  qint64 minimum() const;
  void setMinimum(qint64 minimum);

  qint64 maximum() const;
  void setMaximum(qint64 maximum);

  QValidator::State validate(QString &input, int &pos) const override;

  void fixup(QString &input) const override;

  void stepBy(int steps) override;

  qint64 valueFromText(const QString &text) const;
  QString textFromValue(qint64 value) const;

protected:
  void setRange(qint64 minimum, qint64 maximum);
  QAbstractSpinBox::StepEnabled stepEnabled() const override;

public slots:
  void setValue(qint64 value);

signals:
  void valueChanged(qint64);

private:
  void updateValue();
  void updateEdit();

  qint64 m_value;
  qint64 m_minimum;
  qint64 m_maximum;
  qint64 m_step_value;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_LONGINT_SPINBOX_H_
