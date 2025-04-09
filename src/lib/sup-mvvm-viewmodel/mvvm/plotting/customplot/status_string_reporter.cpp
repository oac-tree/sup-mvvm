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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "status_string_reporter.h"

#include "mouse_move_reporter.h"
#include "mouse_pos_info.h"
#include "status_string_formatter_interface.h"

#include <mvvm/core/mvvm_exceptions.h>

namespace mvvm
{

struct StatusStringReporter::StatusStringReporterImpl
{
  StatusStringReporter* parent{nullptr};
  QCustomPlot* custom_plot{nullptr};
  callback_t callback;
  std::unique_ptr<StatusStringFormatterInterface> fmt;
  std::unique_ptr<MouseMoveReporter> mouse_reporter;
  MousePosInfo prevPos;

  StatusStringReporterImpl(StatusStringReporter* parent, QCustomPlot* custom_plot,
                           callback_t callback,
                           std::unique_ptr<StatusStringFormatterInterface> formatter)
      : parent(parent)
      , custom_plot(custom_plot)
      , callback(std::move(callback))
      , fmt(std::move(formatter))
  {
    if (!custom_plot)
    {
      throw RuntimeException("StatusStringReporter: not initialized custom plot.");
    }

    auto on_mouse_move = [this](const MousePosInfo& pos)
    {
      if (pos.in_axes_range)
      {
        notify_client(pos);
        if (!prevPos.in_axes_range)
          entering_the_area();
      }
      else
      {
        if (prevPos.in_axes_range)
          leaving_the_area();
      }

      prevPos = pos;
    };
    mouse_reporter = std::make_unique<MouseMoveReporter>(custom_plot, on_mouse_move);
  }

  //! Notify client about mouse move with formatted status string.

  void notify_client(const MousePosInfo& pos)
  {
    callback(fmt->status_string(this->custom_plot, pos.xpos, pos.ypos));
  }

  //! Notify client on leaving axes area.

  void leaving_the_area()
  {
    // notifying client with empty string as a sign that we have left the area
    callback({});
  }

  //! Notify client on entering axes area.

  void entering_the_area()
  {
    // for future improvements
  }
};

StatusStringReporter::StatusStringReporter(
    QCustomPlot* custom_plot, callback_t callback,
    std::unique_ptr<StatusStringFormatterInterface> formatter)
    : p_impl(std::make_unique<StatusStringReporterImpl>(this, custom_plot, callback,
                                                        std::move(formatter)))
{
}

StatusStringReporter::~StatusStringReporter() = default;

}  // namespace mvvm
