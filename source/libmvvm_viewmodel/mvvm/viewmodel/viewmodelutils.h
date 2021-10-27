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

#ifndef MVVM_VIEWMODEL_VIEWMODELUTILS_H
#define MVVM_VIEWMODEL_VIEWMODELUTILS_H

#include "mvvm/viewmodel_export.h"

#include <QVector>

namespace ModelView::Utils
{
//! Returns vector of Qt roles corresponding to given ItemDataRole.
MVVM_VIEWMODEL_EXPORT QVector<int> ItemRoleToQtRole(int role);

}  // namespace ModelView::Utils

#endif  // MVVM_VIEWMODEL_VIEWMODELUTILS_H
