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

#include "standard_row_strategies.h"

#include "viewitem_factory.h"

#include <mvvm/model/item_utils.h>
#include <mvvm/model/session_item.h>
#include <mvvm/viewmodelbase/viewitem.h>

namespace mvvm
{

// ----------------------------------------------------------------------------
// LabelDataRowStrategy
// ----------------------------------------------------------------------------

int LabelDataRowStrategy::GetSize() const
{
  return 2;
}

QStringList LabelDataRowStrategy::GetHorizontalHeaderLabels() const
{
  const static QStringList result{"Name", "Value"};
  return result;
}

std::vector<std::unique_ptr<ViewItem>> LabelDataRowStrategy::ConstructRowImpl(SessionItem* item)
{
  // For one SessionItem the row consisting of two ViewItems will be generated: one for displayName,
  // another for SessionItem's data (if defined).
  std::vector<std::unique_ptr<ViewItem>> result;
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

int PropertiesRowStrategy::GetSize() const
{
  return m_current_column_labels.size();
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

std::vector<std::unique_ptr<ViewItem>> PropertiesRowStrategy::ConstructRowImpl(SessionItem* item)
{
  std::vector<std::unique_ptr<ViewItem>> result;

  m_current_column_labels.clear();
  for (auto child : utils::SinglePropertyItems(*item))
  {
    if (child->HasData())
    {
      result.emplace_back(mvvm::CreateDataViewItem(child));
    }
    else
    {
      result.emplace_back(mvvm::CreateDisplayNameViewItem(child));
    }
    m_current_column_labels.push_back(child->GetDisplayName());
  }

  return result;
}

}  // namespace mvvm
