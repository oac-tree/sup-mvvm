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

#ifndef MVVM_EDITORS_FLOAT_SPINBOX_H_
#define MVVM_EDITORS_FLOAT_SPINBOX_H_

#include <mvvm/core/variant.h>
#include <mvvm/viewmodel_export.h>

#include <QVariant>
#include <QWidget>

class QDoubleSpinBox;

namespace mvvm
{

/**
 * @brief The FloatSpinBox class is a spin box editor for mvvm::float32 and mvvm::float64
 * types in Qt's viewmodel-delegate environment.
 *
 * It is an envelope around QDoubleSpinBox. The behavior depends on the type of the initial value.
 * If it is QVariant(float) (which is the result of the conversion of variant_t(mvvm::float32), when
 * QDoubleSpinBox will have min/max limits corresponding to float32. The output value of the editor
 * will be the same as input, i.e. QVariant(float).
 *
 * If the initial value is QVariant(double) (which is the result of the conversion of
 * variant_t(mvvm::float64), when QDoubleSpinBox will have min/max limits corresponding to float64.
 * The output value of the editor will be the same as the input, QVariant(double).
 */
class MVVM_VIEWMODEL_EXPORT FloatSpinBox : public QWidget
{
  Q_OBJECT

  // Thanks to this property model delegates knows how to commit the data using this editor
  Q_PROPERTY(QVariant value MEMBER m_value READ value WRITE setValue NOTIFY valueChanged USER true)

public:
  explicit FloatSpinBox(QWidget* parent_widget = nullptr);

  void setDecimals(int prec);

  QVariant value() const;

  void setValue(const QVariant& value);

  /**
   * @brief Sets allowed range for the value.
   *
   * @details It is user's responsibility to set the proper range while editing QVariant(float32)
   * based values, to not to exceed numeric limits. We do it in FloatEditorBuilder() factory
   * function.
   */
  void SetRange(double lower_limit, double upper_limit);

  double minimum() const;

  double maximum() const;

signals:
  void valueChanged(const QVariant& value);

private:
  /**
   * @brief Provide update of cached value and emit signals, when underlying QDoubleSpinBox has
   * finished its work.
   */
  void OnEditingFinished(double value);

  QDoubleSpinBox* m_double_editor{nullptr};

  QVariant m_value;
  double m_lower_limit{0.0};
  double m_upper_limit{0.0};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_FLOAT_SPINBOX_H_
