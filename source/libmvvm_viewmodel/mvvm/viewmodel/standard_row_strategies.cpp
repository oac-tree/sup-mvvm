/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/viewmodel/standard_row_strategies.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

namespace mvvm
{

// ----------------------------------------------------------------------------
// LabelDataRowStrategy
// ----------------------------------------------------------------------------

QStringList LabelDataRowStrategy::GetHorizontalHeaderLabels() const
{
  return {"Name", "Value"};
}

std::vector<std::unique_ptr<ViewItem>> LabelDataRowStrategy::ConstructRow(SessionItem* item)
{
  std::vector<std::unique_ptr<ViewItem>> result;

  // For one SessionItem the row consisting of two ViewItems will be generated: one for displayName,
  // another for SessionItem's data (if defined).

  if (!item)
  {
    return result;
  }

  result.emplace_back(mvvm::CreateDisplayNameViewItem(item));
  result.emplace_back(mvvm::CreateDataViewItem(item));
  return result;
}

// ----------------------------------------------------------------------------
// PropertiesRowStrategy
// ----------------------------------------------------------------------------

PropertiesRowStrategy::PropertiesRowStrategy(std::vector<std::string> labels)
    : m_user_defined_column_labels(std::move(labels))
{
}

QStringList PropertiesRowStrategy::GetHorizontalHeaderLabels() const
{
  QStringList result;
  auto labels =
      m_user_defined_column_labels.empty() ? m_current_column_labels : m_user_defined_column_labels;
  std::transform(labels.begin(), labels.end(), std::back_inserter(result),
                 [](const std::string& str) { return QString::fromStdString(str); });
  return result;
}

std::vector<std::unique_ptr<ViewItem>> PropertiesRowStrategy::ConstructRow(SessionItem* item)
{
  std::vector<std::unique_ptr<ViewItem>> result;

  if (!item)
  {
    return result;
  }

  auto items_in_row = utils::SinglePropertyItems(*item);
  if (m_user_defined_column_labels.empty())
  {
    UpdateColumnLabels(items_in_row);
  }

  for (auto child : items_in_row)
  {
    if (child->HasData())
    {
      result.emplace_back(mvvm::CreateDataViewItem(child));
    }
    else
    {
      result.emplace_back(mvvm::CreateDisplayNameViewItem(child));
    }
  }

  return result;
}

//! Updates current column labels.

void PropertiesRowStrategy::UpdateColumnLabels(std::vector<SessionItem*> items)
{
  m_current_column_labels.clear();
  std::transform(items.begin(), items.end(), std::back_inserter(m_current_column_labels),
                 [](const SessionItem* item) { return item->GetDisplayName(); });
}

}  // namespace mvvm
