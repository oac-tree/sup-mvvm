/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP and PSPS
 *
 * Description   : Common libraries and tools for Operation Application GUIs
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

#include "chart_viewport_controller.h"

#include "qt_charts.h"

#include <mvvm/plotting/charts/chart_axis_plot_controller.h>
#include <mvvm/plotting/charts/line_series_controller.h>
#include <mvvm/standarditems/axis_items.h>
#include <mvvm/standarditems/chart_viewport_item.h>
#include <mvvm/standarditems/line_series_item.h>
#include <mvvm/standarditems/viewport_item.h>
#include <mvvm/utils/container_utils.h>

namespace mvvm
{

namespace
{

/**
 * @brief Returns animation option encoded in ChartViewportItem.
 */
QChart::AnimationOptions GetAnimationOption(ChartViewportItem *item)
{
  auto values = item->Property<ComboProperty>(ChartViewportItem::kAnimation).GetSelectedValues();

  QChart::AnimationOptions result = QChart::NoAnimation;

  if (utils::Contains(values, ChartViewportItem::kSeriesAnimation))
  {
    result |= QChart::SeriesAnimations;
  }

  if (utils::Contains(values, ChartViewportItem::kGridAnimation))
  {
    result |= QChart::GridAxisAnimations;
  }

  return result;
}

}  // namespace

ChartViewportController::ChartViewportController(QChart *chart) : m_chart(chart) {}

ChartViewportController::~ChartViewportController() = default;

void ChartViewportController::Subscribe()
{
  SetupChart();

  Listener()->Connect<ItemInsertedEvent>(this, &ChartViewportController::OnItemInsertedEvent);
  Listener()->Connect<AboutToRemoveItemEvent>(this,
                                              &ChartViewportController::OnAboutToRemoveItemEvent);
  Listener()->Connect<PropertyChangedEvent>(this, &ChartViewportController::OnPropertyChangedEvent);
}

QAbstractAxis *ChartViewportController::GetXQtAxis() const
{
  return m_x_axis_controller ? m_x_axis_controller->GetQtAxis() : nullptr;
}

QAbstractAxis *ChartViewportController::GetYQtAxis() const
{
  return m_y_axis_controller ? m_y_axis_controller->GetQtAxis() : nullptr;
}

QLineSeries *ChartViewportController::SetupChartForLineSeries(LineSeriesItem *item)
{
  auto qt_line_series = new QLineSeries;
  m_chart->addSeries(qt_line_series);  // ownership is taken by Qt

  auto controller = std::make_unique<LineSeriesController>(qt_line_series);
  controller->SetItem(item);
  m_line_controllers.push_back(std::move(controller));

  return qt_line_series;
}

void ChartViewportController::SetupChart()
{
  m_chart->setAnimationOptions(GetAnimationOption(GetItem()));

  for (auto line_series_item : GetItem()->GetLineSeries())
  {
    SetupChartForLineSeries(line_series_item);
  }

  SetupAxes();
}

void ChartViewportController::SetupAxes()
{
  // creates default axes basing on line series already added to the chart
  m_chart->createDefaultAxes();

  auto x_axes = m_chart->axes(Qt::Horizontal);
  if (!x_axes.empty())
  {
    m_x_axis_controller = std::make_unique<mvvm::ChartAxisPlotController>(x_axes.at(0));
    m_x_axis_controller->SetItem(GetItem()->GetXAxis());
  }

  auto y_axes = m_chart->axes(Qt::Vertical);
  if (!y_axes.empty())
  {
    m_y_axis_controller = std::make_unique<mvvm::ChartAxisPlotController>(y_axes.at(0));
    m_y_axis_controller->SetItem(GetItem()->GetYAxis());
  }
}

void ChartViewportController::OnItemInsertedEvent(const ItemInsertedEvent &event)
{
  const auto [parent, tagindex] = event;
  auto added_child = dynamic_cast<LineSeriesItem *>(parent->GetItem(tagindex));
  auto qt_line_series = SetupChartForLineSeries(added_child);

  if (GetXQtAxis() == nullptr || GetYQtAxis() == nullptr)
  {
    // no axes exist, this is the first LineSeriesItem
    SetupAxes();  // will create axes and attach them to the line series
  }
  else
  {
    // needs attachment, some axes have been already created
    qt_line_series->attachAxis(GetXQtAxis());
    qt_line_series->attachAxis(GetYQtAxis());
  }
}

void ChartViewportController::OnAboutToRemoveItemEvent(const AboutToRemoveItemEvent &event)
{
  const auto [parent, tagindex] = event;
  auto child_about_to_be_removed = dynamic_cast<LineSeriesItem *>(parent->GetItem(tagindex));

  // looking for controller serving item
  auto on_element = [&](const std::unique_ptr<LineSeriesController> &cntrl) -> bool
  { return cntrl->GetItem() == child_about_to_be_removed; };
  auto iter = std::find_if(m_line_controllers.begin(), m_line_controllers.end(), on_element);

  if (iter != m_line_controllers.end())
  {
    auto &controller = *iter;

    // removing Qt's line series from canvas
    auto qt_line_series = controller->GetQtLineSeries();
    m_chart->removeSeries(qt_line_series);  // ownership released by Qt
    delete qt_line_series;

    m_line_controllers.erase(iter);  // removing controllers.
  }
}

void ChartViewportController::OnPropertyChangedEvent(const PropertyChangedEvent &event)
{
  if (event.name == ChartViewportItem::kAnimation)
  {
    m_chart->setAnimationOptions(GetAnimationOption(GetItem()));
  }

  if (event.name == ChartViewportItem::kLegendEnabled)
  {
    m_chart->legend()->setVisible(GetItem()->Property<bool>(ChartViewportItem::kLegendEnabled));
  }
}

}  // namespace mvvm
