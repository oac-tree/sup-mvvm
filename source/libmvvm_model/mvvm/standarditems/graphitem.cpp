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

void GraphItem::SetDataItem(const Data1DItem* data_item)
{
  GetItem<LinkedItem>(P_LINK)->SetLink(data_item);
}

//! Update item from the content of given graph. Link to the data will be set
//! as in given item, other properties copied.

void GraphItem::SetFromGraphItem(const GraphItem* graph_item)
{
  SetDataItem(graph_item->GetDataItem());
  auto pen = GetItem<PenItem>(P_PEN);
  auto source_pen = graph_item->GetItem<PenItem>(P_PEN);
  pen->SetNamedColor(source_pen->GetNamedColor());
  pen->SetWidth(source_pen->GetWidth());
  pen->SetStyle(source_pen->GetStyle());
}

//! Returns data item linked to the given GraphItem.

Data1DItem* GraphItem::GetDataItem() const
{
  return GetItem<LinkedItem>(P_LINK)->Get<Data1DItem>();
}

std::vector<double> GraphItem::GetBinCenters() const
{
  return GetDataItem() ? GetDataItem()->GetBinCenters() : std::vector<double>();
}

std::vector<double> GraphItem::GetValues() const
{
  return GetDataItem() ? GetDataItem()->GetValues() : std::vector<double>();
}

std::vector<double> GraphItem::GetErrors() const
{
  return GetDataItem() ? GetDataItem()->GetErrors() : std::vector<double>();
}

//! Returns color name in #RRGGBB format.

std::string GraphItem::GetNamedColor() const
{
  return GetPenItem()->GetNamedColor();
}

//! Sets named color following schema from https://www.w3.org/TR/css-color-3/#svg-color.
//! e.g. "mediumaquamarine"

void GraphItem::SetNamedColor(const std::string& named_color)
{
  GetPenItem()->SetNamedColor(named_color);
}

PenItem* GraphItem::GetPenItem() const
{
  return GetItem<PenItem>(P_PEN);
}

}  // namespace mvvm
