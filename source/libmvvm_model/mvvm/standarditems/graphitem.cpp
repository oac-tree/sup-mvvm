/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/standarditems/graphitem.h"

#include "mvvm/model/comboproperty.h"
#include "mvvm/standarditems/data1ditem.h"
#include "mvvm/standarditems/linkeditem.h"
#include "mvvm/standarditems/plottableitems.h"

namespace mvvm
{

GraphItem::GraphItem(const std::string& model_type) : CompoundItem(model_type)
{
  AddProperty<LinkedItem>(P_LINK)->SetDisplayName("Link");
  AddProperty<TextItem>(P_GRAPH_TITLE)->SetDisplayName("Graph title");
  AddProperty<PenItem>(P_PEN)->SetDisplayName("Pen");
  AddProperty(P_DISPLAYED, true)->SetDisplayName("Displayed");
}

//! Sets link to the data item.

void GraphItem::setDataItem(const Data1DItem* data_item)
{
  GetItem<LinkedItem>(P_LINK)->SetLink(data_item);
}

//! Update item from the content of given graph. Link to the data will be set
//! as in given item, other properties copied.

void GraphItem::setFromGraphItem(const GraphItem* graph_item)
{
  setDataItem(graph_item->dataItem());
  auto pen = GetItem<PenItem>(P_PEN);
  auto source_pen = graph_item->GetItem<PenItem>(P_PEN);
  pen->SetNamedColor(source_pen->GetNamedColor());
  pen->SetWidth(source_pen->GetWidth());
  pen->SetStyle(source_pen->GetStyle());
}

//! Returns data item linked to the given GraphItem.

Data1DItem* GraphItem::dataItem() const
{
  return GetItem<LinkedItem>(P_LINK)->Get<Data1DItem>();
}

std::vector<double> GraphItem::binCenters() const
{
  return dataItem() ? dataItem()->GetBinCenters() : std::vector<double>();
}

std::vector<double> GraphItem::binValues() const
{
  return dataItem() ? dataItem()->GetValues() : std::vector<double>();
}

std::vector<double> GraphItem::binErrors() const
{
  return dataItem() ? dataItem()->GetErrors() : std::vector<double>();
}

//! Returns color name in #RRGGBB format.

std::string GraphItem::colorName() const
{
  return penItem()->GetNamedColor();
}

//! Sets named color following schema from https://www.w3.org/TR/css-color-3/#svg-color.
//! e.g. "mediumaquamarine"

void GraphItem::setNamedColor(const std::string& named_color)
{
  penItem()->SetNamedColor(named_color);
}

PenItem* GraphItem::penItem() const
{
  return GetItem<PenItem>(P_PEN);
}

}  // namespace mvvm
