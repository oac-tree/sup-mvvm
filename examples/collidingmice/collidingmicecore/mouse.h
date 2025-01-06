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

#ifndef COLLIDINGMICECORE_MOUSE_H
#define COLLIDINGMICECORE_MOUSE_H

#include <QGraphicsItem>

namespace collidingmice
{

class MouseItem;

/**
 * @brief The Mouse class is a slightly modified class from Qt "collidngmice" example.
 *
 * Contains mostly their code, and some model/view interface.
 */
class Mouse : public QGraphicsItem
{
public:
  explicit Mouse(MouseItem* item);

  QRectF boundingRect() const override;
  QPainterPath shape() const override;
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
  void advance(int step) override;

private:
  qreal mouseEyeDirection;
  QColor color;
  MouseItem* mouse_item{nullptr};
};

}  // namespace collidingmice

#endif  // COLLIDINGMICECORE_MOUSE_H
