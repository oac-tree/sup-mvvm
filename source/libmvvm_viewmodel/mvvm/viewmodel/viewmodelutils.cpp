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

#include "mvvm/viewmodel/viewmodelutils.h"

#include "mvvm/model/mvvm_types.h"

namespace mvvm::Utils
{
QVector<int> ItemRoleToQtRole(int role)
{
  QVector<int> result;
  // In Qt when we are editing the data in a view two roles are emmited.
  if (role == DataRole::kDisplay || role == DataRole::kData)
  {
    result = {Qt::DisplayRole, Qt::EditRole};
  }
  else if (role == DataRole::kAppearance)
  {
    result = {Qt::ForegroundRole};
  }
  else if (role == DataRole::kTooltip)
  {
    result = {Qt::ToolTipRole};
  }

  return result;
}

}  // namespace mvvm::Utils
