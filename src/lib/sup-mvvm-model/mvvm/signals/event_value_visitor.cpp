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

#include "event_value_visitor.h"

#include <mvvm/model/session_item.h>

#include <sstream>

namespace mvvm
{

namespace
{

std::string TagIndexString(const TagIndex& tag_index)
{
  return "{\"" + tag_index.GetTag() + "\", " + std::to_string(tag_index.GetIndex()) + "}";
}

std::string ItemToString(const SessionItem* item)
{
  std::ostringstream ostr;
  if (item != nullptr)
  {
    ostr << (item);
    ostr << " " << "[" + item->GetDisplayName() + "]";
  }
  else
  {
    ostr << "nullptr";
  }
  return ostr.str();
}

}  // namespace

std::string EventValueVisitor::operator()(const DataChangedEvent& value)
{
  std::ostringstream ostr;
  ostr << "DataChangedEvent ";
  ostr << ItemToString(value.item) << " " << value.data_role;
  return ostr.str();
}

std::string EventValueVisitor::operator()(const PropertyChangedEvent& value)
{
  std::ostringstream ostr;
  ostr << "PropertyChangedEvent ";
  ostr << ItemToString(value.item) << " " << value.name;
  return ostr.str();
}

std::string EventValueVisitor::operator()(const AboutToInsertItemEvent& value)
{
  std::ostringstream ostr;
  ostr << "AboutToInsertItemEvent ";
  ostr << ItemToString(value.item) << " " << TagIndexString(value.tag_index);
  return ostr.str();
}

std::string EventValueVisitor::operator()(const ItemInsertedEvent& value)
{
  std::ostringstream ostr;
  ostr << "ItemInsertedEvent ";
  ostr << ItemToString(value.item) << " " << TagIndexString(value.tag_index);
  return ostr.str();
}

std::string EventValueVisitor::operator()(const AboutToRemoveItemEvent& value)
{
  std::ostringstream ostr;
  ostr << "AboutToRemoveItemEvent ";
  ostr << ItemToString(value.item) << " " << TagIndexString(value.tag_index);
  return ostr.str();
}

std::string EventValueVisitor::operator()(const ItemRemovedEvent& value)
{
  std::ostringstream ostr;
  ostr << "ItemRemovedEvent ";
  ostr << ItemToString(value.item) << " " << TagIndexString(value.tag_index);
  return ostr.str();
}

std::string EventValueVisitor::operator()(const ItemDestroyedEvent& value)
{
  std::ostringstream ostr;
  ostr << "ItemDestroyedEvent ";
  ostr << ItemToString(value.item);
  return ostr.str();
}

std::string EventValueVisitor::operator()(const ModelAboutToBeResetEvent& value)
{
  std::ostringstream ostr;
  ostr << "ModelAboutToBeResetEvent " << (value.model);
  return ostr.str();
}

std::string EventValueVisitor::operator()(const ModelResetEvent& value)
{
  std::ostringstream ostr;
  ostr << "ModelResetEvent " << (value.model);
  return ostr.str();
}

std::string EventValueVisitor::operator()(const ModelAboutToBeDestroyedEvent& value)
{
  std::ostringstream ostr;
  ostr << "ModelAboutToBeDestroyedEvent " << (value.model);
  return ostr.str();
}

std::string EventToString(const event_variant_t& value)
{
  return std::visit(EventValueVisitor{}, value);
}

}  // namespace mvvm
