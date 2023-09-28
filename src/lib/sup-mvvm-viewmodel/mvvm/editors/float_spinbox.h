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

#ifndef MVVM_EDITORS_FLOAT_SPINBOX_H_
#define MVVM_EDITORS_FLOAT_SPINBOX_H_

#include <mvvm/core/variant.h>
#include <mvvm/viewmodel_export.h>

#include <QAbstractSpinBox>

class QDoubleSpinBox;

namespace mvvm
{

/**
 * @brief The FloatSpinBox class is a spin box editor for mvvm::float32 and mvvm::float64
 * types in Qt's viewmodel-delegate environment.
 *
 * @details It is an envelop around QDoubleSpinBox. The behavior depends on the type of the initial
 * value. If it is QVariant(float) (which is the result of the conversion of
 * variant_t(mvvm::float32), when QDoubleSpinBox will have min/max limits corresponding to float32.
 * The output value of the editor will be the same as input, i.e. QVariant(float).
 *
 * @details If initial value is QVariant(double) (which is the result of the conversion of
 * variant_t(mvvm::float64), when QDoubleSpinBox will have min/max limits corresponding to float64.
 * The output value of the editor will be the same as input, QVariant(double).
 */

class MVVM_VIEWMODEL_EXPORT FloatSpinBox : public QAbstractSpinBox
{
  Q_OBJECT
  Q_PROPERTY(QVariant value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
  explicit FloatSpinBox(QWidget* parent = nullptr);

  void setDecimals(int prec);

  QVariant value() const;

  void setValue(const QVariant& value);

  void SetRange(double lower_limit, double upper_limit);

signals:
  void valueChanged(const QVariant& value);

private:
  void OnEditingFinished(double value);

  QDoubleSpinBox* m_double_editor{nullptr};

  QVariant m_value;
  double m_lower_limit;
  double m_upper_limit;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_FLOAT_SPINBOX_H_
