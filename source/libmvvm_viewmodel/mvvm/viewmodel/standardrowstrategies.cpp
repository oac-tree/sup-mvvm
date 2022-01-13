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

#include "mvvm/viewmodel/standardrowstrategies.h"

#include "mvvm/viewmodel/viewitemfactory.h"
#include "mvvm/viewmodelbase/viewitem.h"

namespace mvvm
{
LabelDataRowStrategy::~LabelDataRowStrategy() = default;

LabelDataRowStrategy::LabelDataRowStrategy(const item_setdata_function_t& set_func)
    : m_set_func(set_func)
{
}

QStringList LabelDataRowStrategy::GetHorizontalHeaderLabels() const
{
  return QStringList() << "Name"
                       << "Value";
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
  result.emplace_back(mvvm::CreateDataViewItem(item, m_set_func));
  return result;
}

}  // namespace mvvm
