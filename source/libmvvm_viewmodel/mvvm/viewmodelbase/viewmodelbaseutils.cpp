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

#include "mvvm/viewmodelbase/viewmodelbaseutils.h"

#include "mvvm/viewmodelbase/viewmodelbase.h"

namespace mvvm::Utils
{
void iterate_model(const QAbstractItemModel* model, const QModelIndex& parent,
                   const std::function<void(const QModelIndex& child)>& fun)
{
  if (!model)
  {
    return;
  }

  for (int row = 0; row < model->rowCount(parent); ++row)
  {
    for (int col = 0; col < model->columnCount(parent); ++col)
    {
      auto index = model->index(row, col, parent);
      if (index.isValid())
      {
        fun(index);
      }
    }
    for (int col = 0; col < model->columnCount(parent); ++col)
    {
      auto index = model->index(row, col, parent);
      iterate_model(model, index, fun);
    }
  }
}

}  // namespace mvvm::Utils
