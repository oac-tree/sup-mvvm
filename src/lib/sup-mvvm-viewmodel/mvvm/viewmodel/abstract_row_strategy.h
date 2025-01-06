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

#ifndef MVVM_VIEWMODEL_ABSTRACT_ROW_STRATEGY_H_
#define MVVM_VIEWMODEL_ABSTRACT_ROW_STRATEGY_H_

#include <mvvm/viewmodel/i_row_strategy.h>

namespace mvvm
{

/**
 * @brief The AbstractRowStrategy class is a base for all strategies to construct the row of view
 * items for a given SessionItem.
 *
 * It provides additional checks for constructed rows via non-virtual-interface idiom.
 */
class AbstractRowStrategy : public IRowStrategy
{
public:

  QStringList GetHorizontalHeaderLabels() const override;

  std::vector<std::unique_ptr<ViewItem>> ConstructRow(SessionItem* item) override;

  /**
   * @brief Returns a placeholder row of fixed size.
   *
   * The row will contain a label with the item's display name as the first element, and empty view
   * items after that. The length of the row will match GetSize() method. This method shall be used
   * in the case when AbstractRowStrategy receives an item that it can't handle. Having a
   * placeholder row should help to debug the tree layout when looking in QTreeView.
   */
  std::vector<std::unique_ptr<ViewItem>> CreatePlaceholderRow(SessionItem *item) const;

private:
  virtual std::vector<std::unique_ptr<ViewItem>> ConstructRowImpl(SessionItem*) = 0;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_ABSTRACT_ROW_STRATEGY_H_
