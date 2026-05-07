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

#include "mock_viewitem_data.h"

namespace mvvm::test
{

QVariant ViewItemDataDecorator::Data(role_t qt_role) const
{
  return m_decoratee->Data(qt_role);
}

bool ViewItemDataDecorator::SetData(const QVariant& value, role_t qt_role)
{
  return m_decoratee->SetData(value, qt_role);
}

bool ViewItemDataDecorator::IsEnabled() const
{
  return m_decoratee->IsEnabled();
}

bool ViewItemDataDecorator::IsEditable() const
{
  return m_decoratee->IsEditable();
}

bool ViewItemDataDecorator::IsCheckable() const
{
  return m_decoratee->IsCheckable();
}

}  // namespace mvvm::test
