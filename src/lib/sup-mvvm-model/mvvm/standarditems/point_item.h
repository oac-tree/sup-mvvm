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

#ifndef MVVM_STANDARDITEMS_POINT_ITEM_H_
#define MVVM_STANDARDITEMS_POINT_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace mvvm
{

/**
 * @brief The PointItem class represents a point on X,Y plane.
 *
 * For the moment PointItem contains two properties coresponding to "x" and "y" coordinates. In the
 * future, more properties can be added, e.g. point color, point size and marker type.
 */
class MVVM_MODEL_EXPORT PointItem : public CompoundItem
{
public:
  static inline const std::string Type = "Point";

  PointItem();

  using CompoundItem::CompoundItem;

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  /**
   * @brief Returns x-coordinate of the point.
   */
  double GetX() const;

  /**
   * @brief Sets x-coordinate of the point to a given value.
   */
  void SetX(double value);

  /**
   * @brief Returns y-coordinate of the point.
   */
  double GetY() const;

  /**
   * @brief Sets y-coordinate of the point to a given value.
   */
  void SetY(double value);

  /**
   * @brief Returns coordinates of point with given index.
   */
  std::pair<double, double> GetPointCoordinates() const;

  /**
   * @brief Set x,y to given value.
   */
  void SetPointCoordinates(const std::pair<double, double>& coordinates);
};

}  // namespace mvvm

#endif  // MVVM_STANDARDITEMS_LINKED_ITEM_H_
