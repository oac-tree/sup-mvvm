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

#ifndef MVVM_VIEWMODEL_STANDARD_ROW_STRATEGIES_H_
#define MVVM_VIEWMODEL_STANDARD_ROW_STRATEGIES_H_

#include <mvvm/interfaces/row_strategy_interface.h>
#include <mvvm/viewmodel_export.h>

#include <vector>

namespace mvvm
{

class SessionItem;
class ViewItem;

/**
 * @brief The LabelDataRowStrategy class constructs the rows with label and data for given
 * SessionItem.
 *
 * @details Row consists of two columns, ViewLabelItem for SessionItem's read-only display role and
 * ViewDataItem for Session's item data role.
 */
class MVVM_VIEWMODEL_EXPORT LabelDataRowStrategy : public RowStrategyInterface
{
public:
  QStringList GetHorizontalHeaderLabels() const override;

  std::vector<std::unique_ptr<ViewItem>> ConstructRow(SessionItem* item) override;
};

/**
 * @brief The PropertiesRowStrategy class constructs the row with columns representing all
 * properties of the given SessionItem.
 */
class MVVM_VIEWMODEL_EXPORT PropertiesRowStrategy : public RowStrategyInterface
{
public:
  explicit PropertiesRowStrategy(std::vector<std::string> labels = {});

  QStringList GetHorizontalHeaderLabels() const override;

  std::vector<std::unique_ptr<ViewItem>> ConstructRow(SessionItem* item) override;

private:
  /**
   * @brief Updates current column labels using display names of given items.
   */
  void UpdateColumnLabels(std::vector<SessionItem*> items);

  std::vector<std::string> m_current_column_labels;
  std::vector<std::string> m_user_defined_column_labels;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_STANDARD_ROW_STRATEGIES_H_
