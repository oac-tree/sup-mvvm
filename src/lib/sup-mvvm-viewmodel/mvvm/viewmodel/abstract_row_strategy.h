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

#ifndef MVVM_VIEWMODEL_ABSTRACT_ROW_STRATEGY_H_
#define MVVM_VIEWMODEL_ABSTRACT_ROW_STRATEGY_H_

#include <mvvm/interfaces/row_strategy_interface.h>

namespace mvvm
{

/**
 * @brief The AbstractRowStrategy class is a base for all strategies to construct the row of view
 * items for a given SessionItem.
 *
 * It provides additional checks for constructed rows via non-virtual-interface idiom.
 */
class AbstractRowStrategy : public RowStrategyInterface
{
public:
  virtual int GetSize() const override;

  std::vector<std::unique_ptr<ViewItem>> ConstructRow(SessionItem* item) override;

private:
  virtual std::vector<std::unique_ptr<ViewItem>> ConstructRowImpl(SessionItem*) = 0;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ABSTRACT_ROW_STRATEGY_H_
