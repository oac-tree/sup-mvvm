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

#include "custom_plot_utils.h"

#include <qcustomplot.h>

namespace mvvm::utils
{

void SetLogarithmicScale(QCPColorScale* axis, bool is_log_scale)
{
  if (is_log_scale && axis->dataScaleType() != QCPAxis::stLogarithmic)
  {
    axis->setDataScaleType(QCPAxis::stLogarithmic);
  }
  else if (!is_log_scale && axis->dataScaleType() != QCPAxis::stLinear)
  {
    axis->setDataScaleType(QCPAxis::stLinear);
  }

  SetLogarithmicScale(axis->axis(), is_log_scale);
}

void SetLogarithmicScale(QCPAxis* axis, bool is_log_scale)
{
  if (is_log_scale)
  {
    axis->setNumberFormat("eb");
    axis->setNumberPrecision(0);
    axis->setScaleType(QCPAxis::stLogarithmic);
    QSharedPointer<QCPAxisTicker> ticker(new QCPAxisTickerLog);
    axis->setTicker(ticker);
  }
  else
  {
    axis->setNumberFormat("g");
    axis->setNumberPrecision(6);
    axis->setScaleType(QCPAxis::stLinear);
    QSharedPointer<QCPAxisTicker> ticker(new QCPAxisTicker);
    axis->setTicker(ticker);
  }
}

}  // namespace mvvm::utils
