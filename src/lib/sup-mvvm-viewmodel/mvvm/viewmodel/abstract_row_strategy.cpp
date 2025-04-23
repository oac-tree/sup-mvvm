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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "abstract_row_strategy.h"

#include <mvvm/core/mvvm_exceptions.h>
#include <mvvm/model/session_item.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <sstream>

namespace mvvm
{

QStringList AbstractRowStrategy::GetHorizontalHeaderLabels() const
{
  return {};
}

std::vector<std::unique_ptr<ViewItem>> AbstractRowStrategy::ConstructRow(SessionItem *item)
{
  if (!item)
  {
    return {};
  }

  auto result = ConstructRowImpl(item);

  // If the result is empty, we silently create a placeholder row.
  if (result.empty())
  {
    return CreatePlaceholderRow(item);
  }

  // If user implementation creates a non-empty row with wrong size, then probably something went
  // wrong.
  if (result.size() != GetSize())
  {
    std::ostringstream ostr;
    ostr << "Size of generated row [" << result.size() << "] ";
    ostr << "for item with type=[" + item->GetType() << "], ";
    ostr << "name=[" + item->GetDisplayName() << "] ";
    ostr << "doesn't coincide with size reported by the strategy [" << GetSize() << "]";
    throw RuntimeException(ostr.str());
  }

  return ConstructRowImpl(item);
}

std::vector<std::unique_ptr<ViewItem>> AbstractRowStrategy::CreatePlaceholderRow(
    SessionItem *item) const
{
  std::vector<std::unique_ptr<mvvm::ViewItem>> result;

  if (GetSize() > 0)
  {
    result.emplace_back(mvvm::CreateDisplayNameViewItem(item));
    for (std::size_t i_row = 1; i_row < GetSize(); ++i_row)
    {
      result.emplace_back(mvvm::CreateLabelViewItem(item));
    }
  }

  return result;
}

}  // namespace mvvm
