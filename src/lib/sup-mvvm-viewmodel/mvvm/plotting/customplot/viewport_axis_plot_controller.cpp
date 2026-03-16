/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "viewport_axis_plot_controller.h"

#include "axis_title_controller.h"
#include "custom_plot_utils.h"

#include <mvvm/model/item_constants.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/plottable_items.h>

#include <qcustomplot.h>

#include <QObject>

namespace mvvm
{

struct ViewportAxisPlotController::AxesPlotControllerImpl
{
  ViewportAxisPlotController* m_self{nullptr};
  QCPAxis* m_axis{nullptr};
  bool m_block_update{false};
  std::unique_ptr<QMetaObject::Connection> m_axis_connection;
  std::unique_ptr<AxisTitleController> m_title_controller;

  AxesPlotControllerImpl(ViewportAxisPlotController* controller, QCPAxis* axis)
      : m_self(controller), m_axis(axis)
  {
    if (axis == nullptr)
    {
      throw RuntimeException("AxisPlotController: axis is not initialized");
    }
    m_axis_connection = std::make_unique<QMetaObject::Connection>();
  }

  AxesPlotControllerImpl(const AxesPlotControllerImpl&) = delete;
  AxesPlotControllerImpl& operator=(const AxesPlotControllerImpl&) = delete;
  AxesPlotControllerImpl(AxesPlotControllerImpl&&) noexcept = delete;
  AxesPlotControllerImpl& operator=(AxesPlotControllerImpl&&) noexcept = delete;

  //! Connects QCustomPlot signals with controller methods.
  void SetConnected()
  {
    auto on_axis_range = [this](const QCPRange& newRange)
    {
      m_block_update = true;
      auto item = m_self->GetItem();
      item->SetRange(newRange.lower, newRange.upper);
      m_block_update = false;
    };

    *m_axis_connection = QObject::connect(
        m_axis, static_cast<void (QCPAxis::*)(const QCPRange&)>(&QCPAxis::rangeChanged),
        on_axis_range);
  }

  //! Disonnects QCustomPlot signals.

  void SetDisconnected() const { (void)QObject::disconnect(*m_axis_connection); }

  //! Sets axesRange from SessionItem.
  void SetAxisRangeFromItem() const
  {
    auto [lower, upper] = m_self->GetItem()->GetRange();
    m_axis->setRange(QCPRange(lower, upper));
  }

  //! Sets log scale from item.

  void SetAxisLogScaleFromItem()
  {
    utils::SetLogarithmicScale(m_axis, m_self->GetItem()->IsInLog());
  }

  //! Init axis from item and setup connections.

  void InitAxis()
  {
    m_title_controller = std::make_unique<AxisTitleController>(m_axis);
    auto text_item = m_self->GetItem()->GetTitleItem();
    m_title_controller->SetItem(text_item);
    SetAxisRangeFromItem();
    SetAxisLogScaleFromItem();
    SetConnected();
  }

  void UpdateLowerRange(const ViewportAxisItem* item)
  {
    SetDisconnected();
    m_axis->setRangeLower(item->GetMin());
    SetConnected();
  }

  void UpdateUpperRange(const ViewportAxisItem* item)
  {
    SetDisconnected();
    m_axis->setRangeUpper(item->GetMax());
    SetConnected();
  }

  ~AxesPlotControllerImpl() { SetDisconnected(); }
};

ViewportAxisPlotController::ViewportAxisPlotController(QCPAxis* axis)
    : p_impl(std::make_unique<AxesPlotControllerImpl>(this, axis))

{
}

ViewportAxisPlotController::~ViewportAxisPlotController() = default;

void ViewportAxisPlotController::Subscribe()
{
  auto on_property_change = [this](const PropertyChangedEvent& event)
  {
    if (p_impl->m_block_update)
    {
      return;
    }

    if (event.name == constants::kMin)
    {
      p_impl->UpdateLowerRange(GetItem());
    }

    if (event.name == constants::kMax)
    {
      p_impl->UpdateUpperRange(GetItem());
    }

    if (event.name == constants::kIsLog)
    {
      p_impl->SetAxisLogScaleFromItem();
    }

    p_impl->m_axis->parentPlot()->replot();
  };
  Listener()->Connect<PropertyChangedEvent>(on_property_change);

  p_impl->InitAxis();
}

void ViewportAxisPlotController::Unsubscribe()
{
  p_impl->SetDisconnected();
}

}  // namespace mvvm
