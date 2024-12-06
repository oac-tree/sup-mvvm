/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_PLOTTING_CHARTS_CHART_AXIS_TITLE_CONTROLLER_H_
#define MVVM_PLOTTING_CHARTS_CHART_AXIS_TITLE_CONTROLLER_H_

#include <mvvm/signals/item_controller.h>
#include <mvvm/plotting/charts/qt_charts_fwd.h>

#include <QObject>

namespace mvvm
{

class TextItem;

/**
 * @brief The ChartAxisTitleController class propagates title settings from TextItem to QCPAxis.
 */
class ChartAxisTitleController : public ItemController<TextItem>
{
public:
  explicit ChartAxisTitleController(QAbstractAxis* axis);

  QAbstractAxis* GetQtAxis();

protected:
  void Subscribe() override;

private:
  void UpdateAxisTitleFromItem();

  QAbstractAxis* m_axis{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CHARTS_CHART_AXIS_TITLE_CONTROLLER_H_
