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

#include "property_table_viewmodel.h"

#include "standard_children_strategies.h"
#include "standard_row_strategies.h"

#include <mvvm/factories/viewmodel_controller_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

namespace mvvm
{

PropertyTableViewModel::PropertyTableViewModel(SessionModelInterface* model, QObject* parent)
    : ViewModel(parent)
{
  SetController(factory::CreateController<TopItemsStrategy, PropertiesRowStrategy>(model, this));
}

void PropertyTableViewModel::insertRow(ViewItem* parent, int row,
                                       std::vector<std::unique_ptr<ViewItem>> items)
{
  // The code below is used to inform QTableView about layout change if the number
  // of columns before the insertion doesn't coincide with the length of `items` vector to insert.
  // This happens when PropertyTableViewModel is looking on empty SessionModel.
  int prev_column_count = parent->GetColumnCount();
  ViewModel::insertRow(parent, row, std::move(items));
  if (parent->GetColumnCount() != prev_column_count)
  {
    emit layoutChanged();
  }
}

}  // namespace mvvm
