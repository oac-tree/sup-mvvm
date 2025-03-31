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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mock_cell_decorator.h"

namespace mvvm::test
{

bool CellDecoratorWrapper::HasCustomDecoration(const QModelIndex &index) const
{
  return m_decoratee->HasCustomDecoration(index);
}

void CellDecoratorWrapper::InitStyleOption(const QModelIndex &index,
                                           QStyleOptionViewItem *option) const
{
  m_decoratee->InitStyleOption(index, option);
}

}  // namespace mvvm::test
