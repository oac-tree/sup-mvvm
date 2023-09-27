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
 */

class MVVM_VIEWMODEL_EXPORT AllIntSpinBox : public QAbstractSpinBox
{
  Q_OBJECT

  Q_PROPERTY(QVariant value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
  explicit AllIntSpinBox(QWidget *parent = nullptr);
  ~AllIntSpinBox() override;

  void SetInteger(std::unique_ptr<ILimitedInteger> value);

  QVariant value() const;

  QValidator::State validate(QString &input, int &pos) const override;

  void fixup(QString &input) const override {}

  void stepBy(int steps) override;

protected:
  QAbstractSpinBox::StepEnabled stepEnabled() const override;

public slots:
  void setValue(const QVariant &value);

signals:
  void valueChanged(QVariant);

private:
  void OnEditingFinished();
  void updateEdit();
  void CheckNotify();

  std::unique_ptr<ILimitedInteger> m_value;
  bool m_cached_value_was_changed{false};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_ALLINT_SPINBOX_H_
