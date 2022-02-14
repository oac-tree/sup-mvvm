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

#include "mvvm/viewmodel/standardpresentationitems.h"

#include "mvvm/model/sessionitem.h"

namespace mvvm
{

QVariant SessionItemPresentation::Data(int role) const
{
  return {};
}

bool SessionItemPresentation::SetData(const QVariant &data, int role)
{
  return false;
}

bool SessionItemPresentation::IsEnabled() const
{
  return false;
}

bool SessionItemPresentation::IsEditable() const
{
  return false;
}

SessionItem *SessionItemPresentation::GetItem() const
{
  return m_item;
}

// SessionItemPresentation::SessionItemPresentation(SessionItem *item) : PresentationItem(item) {}

}  // namespace mvvm
