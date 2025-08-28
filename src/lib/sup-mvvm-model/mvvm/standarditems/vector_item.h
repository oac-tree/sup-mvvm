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

#ifndef MVVM_STANDARDITEMS_VECTOR_ITEM_H_
#define MVVM_STANDARDITEMS_VECTOR_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace mvvm
{

/**
 * @brief The VectorItem class is a compound item with three x,y,z property items.
 */
class MVVM_MODEL_EXPORT VectorItem : public CompoundItem
{
public:
  static inline const std::string kX = "kX";
  static inline const std::string kY = "kY";
  static inline const std::string kZ = "kZ";

  VectorItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  double X() const;
  void SetX(double value);

  double Y() const;
  void SetY(double value);

  double Z() const;
  void SetZ(double value);

  void SetXYZ(double x, double y, double Z);

protected:
  void ActivateStrategies() override;

private:
  /**
   * @brief Custom strategy to set property value and update item's label.
   *
   * It is used to automatically change two things at once (the value of the property, and the
   * label of VectorItem), and be able to undo it in one go.
   */
  bool SetPropertyValue(SessionItem* property, const variant_t& variant, role_t role);
  void UpdateLabel();
};

template <>
struct item_traits<VectorItem>
{
  static constexpr std::string_view type_name() noexcept { return "VectorItem"; }
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_VECTOR_ITEM_H_
