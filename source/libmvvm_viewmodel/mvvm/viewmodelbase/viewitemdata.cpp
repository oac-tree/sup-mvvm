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

#include "viewitemdata.h"

using namespace mvvm;

ViewItemData::ViewItemData() : m_is_enabled(true), m_is_editable(true) {}

QVariant ViewItemData::Data(int role) const
{
  return m_data[role];
}

bool ViewItemData::SetData(const QVariant& data, int role)
{
  m_data[role] = data;
  return true;
}

bool ViewItemData::IsEnabled() const
{
  return m_is_enabled;
}

void ViewItemData::SetEnabled(bool value)
{
  m_is_enabled = value;
}

bool ViewItemData::IsEditable() const
{
  return m_is_editable;
}

void ViewItemData::SetEditable(bool value)
{
  m_is_editable = value;
}
