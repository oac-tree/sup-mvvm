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

#ifndef MVVM_STANDARDITEMS_DATA1D_ITEM_H_
#define MVVM_STANDARDITEMS_DATA1D_ITEM_H_

#include <mvvm/model/compound_item.h>

#include <vector>

namespace mvvm
{

class BinnedAxisItem;

//! Represents one-dimensional data (axis and values).
//! Values are stored in Data1DItem itself, axis is attached as a child. Corresponding plot
//! properties will be served by GraphItem.

class MVVM_MODEL_EXPORT Data1DItem : public CompoundItem
{
public:
  static inline const std::string kValues = "kValues";
  static inline const std::string kErrors = "kErrors";

  Data1DItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  std::vector<double> GetBinCenters() const;

  void SetValues(const std::vector<double>& data);
  std::vector<double> GetValues() const;

  void SetErrors(const std::vector<double>& errors);
  std::vector<double> GetErrors() const;

  BinnedAxisItem* GetAxis() const;

  //! Inserts axis of given type.
  template <typename T, typename... Args>
  T* SetAxis(Args&&... args);

  void SetAxis(std::unique_ptr<BinnedAxisItem> axis);
};

template <typename T, typename... Args>
T* Data1DItem::SetAxis(Args&&... args)
{
  auto axis = std::make_unique<T>();
  axis->SetParameters(std::forward<Args>(args)...);
  auto result = axis.get();
  SetAxis(std::move(axis));
  return result;
}

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_DATA1D_ITEM_H_
