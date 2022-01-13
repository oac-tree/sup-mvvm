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

#ifndef MVVM_STANDARDITEMS_VECTORITEM_H
#define MVVM_STANDARDITEMS_VECTORITEM_H

#include "mvvm/model/compounditem.h"

namespace mvvm
{
//! Vector item with three x,y,z property items.

class MVVM_MODEL_EXPORT VectorItem : public CompoundItem
{
public:
  static inline const std::string Type = "VectorItem";
  static inline const std::string P_X = "P_X";
  static inline const std::string P_Y = "P_Y";
  static inline const std::string P_Z = "P_Z";

  VectorItem();

  double X() const;
  void SetX(double value);

  double Y() const;
  void SetY(double value);

  double Z() const;
  void SetZ(double value);

private:
  void UpdateLabel();
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_VECTORITEM_H
