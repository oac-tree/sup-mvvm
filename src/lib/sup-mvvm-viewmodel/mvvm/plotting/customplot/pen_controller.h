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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_PEN_CONTROLLER_H_
#define MVVM_PLOTTING_CUSTOMPLOT_PEN_CONTROLLER_H_

#include <mvvm/signals/item_listener.h>
#include <mvvm/view_export.h>

#include <memory>

class QCPGraph;

namespace mvvm
{

class PenItem;

//! Establishes communication between QCPGraph and PenItem.
//! Provides update of QCPGraph's color, line style and width when PenItem is changed.

class MVVM_VIEW_EXPORT PenController : public ItemController<PenItem>
{
public:
  explicit PenController(QCPGraph* graph);
  ~PenController() override;

protected:
  void Subscribe() override;

private:
  struct PenControllerImpl;
  std::unique_ptr<PenControllerImpl> p_impl;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_PEN_CONTROLLER_H_
