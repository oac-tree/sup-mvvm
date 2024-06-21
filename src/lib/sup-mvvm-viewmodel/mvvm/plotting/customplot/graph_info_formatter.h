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

#ifndef MVVM_PLOTTING_CUSTOMPLOT_GRAPH_INFO_FORMATTER_H_
#define MVVM_PLOTTING_CUSTOMPLOT_GRAPH_INFO_FORMATTER_H_

#include <mvvm/plotting/customplot/status_string_formatter_interface.h>

class QCustomPlot;

namespace mvvm
{

//! Formats status string for current mouse position in QCustomPlot with QCPGraph's.
//! Includes coordinates of mouse pointer in viewport axes coordinates, add graph info if there is
//! one nearby.

class MVVM_VIEW_EXPORT GraphInfoFormatter : public StatusStringFormatterInterface
{
public:
  //! Returns status string representing graph nearby.
  //! @params x: mouse x-position given in axis viewport coordinates
  //! @params y: mouse y-position given in axis viewport coordinates
  std::string status_string(QCustomPlot* custom_plot, double x, double y) const override;
};

}  // namespace mvvm

#endif  // MVVM_PLOTTING_CUSTOMPLOT_GRAPH_INFO_FORMATTER_H_
