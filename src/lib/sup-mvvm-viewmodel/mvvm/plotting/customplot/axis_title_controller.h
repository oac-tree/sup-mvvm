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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_AXIS_TITLE_CONTROLLER_H_
#define MVVM_PLOTTING_CUSTOMPLOT_AXIS_TITLE_CONTROLLER_H_

#include <mvvm/signals/item_controller.h>
#include <mvvm/view_export.h>

#include <memory>

class QCPAxis;

namespace mvvm
{

class TextItem;

/**
 * @brief The AxisTitleController class propagates title settings from TextItem to QCPAxis.
 */
class MVVM_VIEW_EXPORT AxisTitleController : public ItemController<TextItem>
{
public:
  explicit AxisTitleController(QCPAxis* axis);
  ~AxisTitleController() override;

protected:
  void Subscribe() override;

public:
  struct AxisTitleControllerImpl;
  std::unique_ptr<AxisTitleControllerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_AXIS_TITLE_CONTROLLER_H_
