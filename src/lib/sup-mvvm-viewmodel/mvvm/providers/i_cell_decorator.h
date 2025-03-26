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

#ifndef MVVM_PROVIDERS_I_CELL_DECORATOR_H_
#define MVVM_PROVIDERS_I_CELL_DECORATOR_H_

#include <mvvm/viewmodel_export.h>

class QModelIndex;
class QStyleOptionViewItem;

namespace mvvm
{

/**
 * @brief The ICellDecorator class is a base for custom cell decorations of Qt trees and tables.
 *
 * The cell decoration defines how the cell look like before the user double-click on it.
 */
class MVVM_VIEWMODEL_EXPORT ICellDecorator
{
public:
  ICellDecorator() = default;
  virtual ~ICellDecorator() = default;

  ICellDecorator(const ICellDecorator&) = delete;
  ICellDecorator& operator=(const ICellDecorator&) = delete;
  ICellDecorator(ICellDecorator&&) = delete;
  ICellDecorator& operator=(ICellDecorator&&) = delete;

  /**
   * @brief Checks if a cell corresponding to a given index has a custom decoration.
   */
  virtual bool HasCustomDecoration(const QModelIndex& index) const = 0;

  /**
   * @brief Decorate the given style option with our custom decoration.
   */
  virtual void InitStyleOption(const QModelIndex& index, QStyleOptionViewItem* option) const = 0;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_I_CELL_DECORATOR_H_
