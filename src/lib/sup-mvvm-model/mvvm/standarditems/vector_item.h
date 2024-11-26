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
  static inline const std::string Type = "VectorItem";
  static inline const std::string kX = "kX";
  static inline const std::string kY = "kY";
  static inline const std::string kZ = "kZ";

  VectorItem();

  std::unique_ptr<SessionItem> Clone() const override;

  double X() const;
  void SetX(double value);

  double Y() const;
  void SetY(double value);

  double Z() const;
  void SetZ(double value);

  void SetXYZ(double x, double y, double Z);

  /**
   * @details Activates listening on own properties change. It will update own label on any of x,y,z
   * children change.
   */
  void Activate() override;

private:
  void UpdateLabel();
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_VECTOR_ITEM_H_
