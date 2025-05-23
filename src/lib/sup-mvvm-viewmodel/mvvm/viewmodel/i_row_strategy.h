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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_VIEWMODEL_I_ROW_STRATEGY_H_
#define MVVM_VIEWMODEL_I_ROW_STRATEGY_H_

#include <mvvm/viewmodel_export.h>

#include <QStringList>
#include <memory>
#include <vector>

namespace mvvm
{

class SessionItem;
class ViewItem;

/**
 * @brief The IRowStrategy class is an interface for all strategies to construct the row of
 * ViewItems for the given SessionItem.
 *
 * Used in the context of AbstractViewModel while exposing SessionModel to Qt.
 */
class MVVM_VIEWMODEL_EXPORT IRowStrategy
{
public:
  virtual ~IRowStrategy() = default;

  /**
   * @brief Returns number of elements in a row (i.e. model's column count).
   *
   * This number represents a number of columns in the model. It is a constant for all MVVM
   * trees/tables. In opposite to Qt, we do not support dynamic change (i.e. silent insertion or
   * removal) of the number of columns.
   */
  virtual std::size_t GetSize() const = 0;

  /**
   * @brief Returns vector of horizontal labels which view model might want to use.
   */
  virtual QStringList GetHorizontalHeaderLabels() const = 0;

  /**
   * @brief Returns vector of ViewItems for given SessionItem.
   */
  virtual std::vector<std::unique_ptr<ViewItem>> ConstructRow(SessionItem*) = 0;
};

}  // namespace mvvm

#endif  // MVVM_VIEWMODEL_I_ROW_STRATEGY_H_
