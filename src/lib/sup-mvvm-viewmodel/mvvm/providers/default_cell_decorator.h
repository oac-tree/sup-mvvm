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

#ifndef MVVM_PROVIDERS_DEFAULT_CELL_DECORATOR_H_
#define MVVM_PROVIDERS_DEFAULT_CELL_DECORATOR_H_

#include <mvvm/viewmodel/i_cell_decorator.h>

#include <optional>
#include <string>

namespace mvvm
{

//! Generates default cell decorations for Qt trees and tables.

class MVVM_VIEWMODEL_EXPORT DefaultCellDecorator : public ICellDecorator
{
public:
  bool HasCustomDecoration(const QModelIndex& index) const override;
  void InitStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) override;

protected:
  virtual std::optional<std::string> GetCellText(const QModelIndex& index) const;
};

}  // namespace mvvm

#endif  // MVVM_PROVIDERS_DEFAULT_CELL_DECORATOR_H_
