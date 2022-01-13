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

#ifndef MVVM_VIEWMODEL_STANDARDROWSTRATEGIES_H
#define MVVM_VIEWMODEL_STANDARDROWSTRATEGIES_H

#include "mvvm/interfaces/rowstrategyinterface.h"
#include "mvvm/model/function_types.h"
#include "mvvm/viewmodel_export.h"

#include <vector>

namespace mvvm
{
class SessionItem;
class ViewItem;

//! Constructs row of ViewItem's for given SessionItem.
//! Row consists of two columns, ViewLabelItem for SessionItem's display role and
//! ViewDataItem for Session's item data role.

class MVVM_VIEWMODEL_EXPORT LabelDataRowStrategy : public RowStrategyInterface
{
public:
  LabelDataRowStrategy(const item_setdata_function_t& set_func = {});
  ~LabelDataRowStrategy();

  QStringList GetHorizontalHeaderLabels() const override;

  std::vector<std::unique_ptr<ViewItem>> ConstructRow(SessionItem*) override;

private:
  item_setdata_function_t m_set_func;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_STANDARDROWSTRATEGIES_H
