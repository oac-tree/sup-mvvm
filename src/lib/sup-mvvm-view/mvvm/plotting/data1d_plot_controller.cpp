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

#include "data1d_plot_controller.h"

#include <mvvm/signals/item_connect_utils.h>
#include <mvvm/standarditems/data1d_item.h>

#include <qcustomplot.h>

#include <stdexcept>

namespace
{
template <typename T>
QVector<T> fromStdVector(const std::vector<T>& vec)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
  return QVector<T>(vec.begin(), vec.end());
#else
  return QVector<T>::fromStdVector(vec);
#endif
}
}  // namespace

using namespace mvvm;

struct Data1DPlotController::Data1DPlotControllerImpl
{
  QCPGraph* m_graph{nullptr};
  QCPErrorBars* m_error_bars{nullptr};

  explicit Data1DPlotControllerImpl(QCPGraph* graph) : m_graph(graph)
  {
    if (!m_graph)
    {
      throw std::runtime_error("Uninitialised graph in Data1DPlotController");
    }
  }

  void InitGraphFromItem(Data1DItem* item)
  {
    assert(item);
    UpdateGraphPointsFromItem(item);
    UpdateErrorBarsFromItem(item);
  }

  void UpdateGraphPointsFromItem(Data1DItem* item)
  {
    m_graph->setData(fromStdVector<double>(item->GetBinCenters()),
                     fromStdVector<double>(item->GetValues()));
    GetCustomPlot()->replot();
  }

  void UpdateErrorBarsFromItem(Data1DItem* item)
  {
    auto errors = item->GetErrors();
    if (errors.empty())
    {
      ResetErrorBars();
      return;
    }

    if (!m_error_bars)
    {
      m_error_bars = new QCPErrorBars(GetCustomPlot()->xAxis, GetCustomPlot()->yAxis);
    }

    m_error_bars->setData(fromStdVector<double>(errors));
    m_error_bars->setDataPlottable(m_graph);
  }

  void ResetGraph()
  {
    m_graph->setData(QVector<double>{}, QVector<double>{});
    GetCustomPlot()->replot();
  }

  void ResetErrorBars()
  {
    delete m_error_bars;
    m_error_bars = nullptr;
  }

  QCustomPlot* GetCustomPlot()
  {
    assert(m_graph);
    return m_graph->parentPlot();
  }
};

Data1DPlotController::Data1DPlotController(QCPGraph* graph)
    : p_impl(std::make_unique<Data1DPlotControllerImpl>(graph))
{
}

Data1DPlotController::~Data1DPlotController() = default;

void Data1DPlotController::Subscribe()
{
  auto on_property_change = [this](const event_variant_t& event)
  {
    auto concrete_event = std::get<PropertyChangedEvent>(event);
    if (concrete_event.m_name == Data1DItem::kValues)
    {
      p_impl->UpdateGraphPointsFromItem(GetItem());
    }
    if (concrete_event.m_name == Data1DItem::kErrors)
    {
      p_impl->UpdateErrorBarsFromItem(GetItem());
    }
  };
  Connect<PropertyChangedEvent>(on_property_change);

  p_impl->InitGraphFromItem(GetItem());
}

void Data1DPlotController::Unsubscribe()
{
  p_impl->ResetGraph();
}
