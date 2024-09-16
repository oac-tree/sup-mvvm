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

#include "mouse.h"

#include "mousemodel.h"

#include <mvvm/signals/item_connect.h>
#include <mvvm/utils/numeric_utils.h>

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <cmath>

namespace collidingmice
{

namespace
{

const qreal Pi = M_PI;
const qreal TwoPi = 2 * M_PI;

static qreal normalizeAngle(qreal angle)
{
  while (angle < 0)
  {
    angle += TwoPi;
  }
  while (angle > TwoPi)
  {
    angle -= TwoPi;
  }
  return angle;
}

}  // namespace

Mouse::Mouse(MouseItem* item)
    : mouseEyeDirection(0)
    , color(QString::fromStdString(item->Property<std::string>(MouseItem::P_COLOR)))
    , mouse_item(item)
{
  auto on_property_change = [this](const mvvm::PropertyChangedEvent& event)
  {
    if (event.m_name == MouseItem::P_XPOS)
    {
      setX(mouse_item->Property<double>(MouseItem::P_XPOS));
    }
    if (event.m_name == MouseItem::P_YPOS)
    {
      setY(mouse_item->Property<double>(MouseItem::P_YPOS));
    }
    if (event.m_name == MouseItem::P_COLOR)
    {
      color = QColor(QString::fromStdString(mouse_item->Property<std::string>(MouseItem::P_COLOR)));
      update();
    }
    if (event.m_name == MouseItem::P_ANGLE)
    {
      qreal dx = std::sin(mouse_item->Property<double>(MouseItem::P_ANGLE)) * 10;
      setRotation(rotation() + dx);
    }
  };
  mvvm::connect::Connect<mvvm::PropertyChangedEvent>(mouse_item, on_property_change);

  setPos(item->Property<double>(MouseItem::P_XPOS), item->Property<double>(MouseItem::P_YPOS));
  setRotation(mvvm::utils::RandInt(0, 360 * 16));
}

QRectF Mouse::boundingRect() const
{
  qreal adjust = 0.5;
  return QRectF(-18 - adjust, -22 - adjust, 36 + adjust, 60 + adjust);
}

QPainterPath Mouse::shape() const
{
  QPainterPath path;
  path.addRect(-10, -20, 20, 40);
  return path;
}

void Mouse::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
  // Body
  painter->setBrush(color);
  painter->drawEllipse(-10, -20, 20, 40);

  // Eyes
  painter->setBrush(Qt::white);
  painter->drawEllipse(-10, -17, 8, 8);
  painter->drawEllipse(2, -17, 8, 8);

  // Nose
  painter->setBrush(Qt::black);
  painter->drawEllipse(QRectF(-2, -22, 4, 4));

  // Pupils
  painter->drawEllipse(QRectF(-8.0 + mouseEyeDirection, -17, 4, 4));
  painter->drawEllipse(QRectF(4.0 + mouseEyeDirection, -17, 4, 4));

  // Ears
  painter->setBrush(scene()->collidingItems(this).isEmpty() ? Qt::darkYellow : Qt::red);
  painter->drawEllipse(-17, -12, 16, 16);
  painter->drawEllipse(1, -12, 16, 16);

  // Tail
  QPainterPath path(QPointF(0, 20));
  path.cubicTo(-5, 22, -5, 22, 0, 25);
  path.cubicTo(5, 27, 5, 32, 0, 30);
  path.cubicTo(-5, 32, -5, 42, 0, 35);
  painter->setBrush(Qt::NoBrush);
  painter->drawPath(path);
}

void Mouse::advance(int step)
{
  if (!step)
  {
    return;
  }
  // Don't move too far away

  qreal angle = mouse_item->Property<double>(MouseItem::P_ANGLE);

  QLineF lineToCenter(QPointF(0, 0), mapFromScene(0, 0));
  if (lineToCenter.length() > 150)
  {
    qreal angleToCenter = std::atan2(lineToCenter.dy(), lineToCenter.dx());
    angleToCenter = normalizeAngle((Pi - angleToCenter) + Pi / 2);

    if (angleToCenter < Pi && angleToCenter > Pi / 4)
    {
      // Rotate left
      angle += (angle < -Pi / 2) ? 0.25 : -0.25;
    }
    else if (angleToCenter >= Pi && angleToCenter < (Pi + Pi / 2 + Pi / 4))
    {
      // Rotate right
      angle += (angle < Pi / 2) ? 0.25 : -0.25;
    }
  }
  else if (::sin(angle) < 0)
  {
    angle += 0.25;
  }
  else if (::sin(angle) > 0)
  {
    angle -= 0.25;
  }

  // Try not to crash with any other mice
  QList<QGraphicsItem*> dangerMice = scene()->items(
      QPolygonF() << mapToScene(0, 0) << mapToScene(-30, -50) << mapToScene(30, -50));
  foreach (QGraphicsItem* item, dangerMice)
  {
    if (item == this)
    {
      continue;
    }

    const QLineF lineToMouse(QPointF(0, 0), mapFromItem(item, 0, 0));
    qreal angleToMouse = std::atan2(lineToMouse.dy(), lineToMouse.dx());
    angleToMouse = normalizeAngle((Pi - angleToMouse) + Pi / 2);

    if (angleToMouse >= 0 && angleToMouse < Pi / 2)
    {
      // Rotate right
      angle += 0.5;
    }
    else if (angleToMouse <= TwoPi && angleToMouse > (TwoPi - Pi / 2))
    {
      // Rotate left
      angle -= 0.5;
    }
  }

  // Add some random movement
  if (dangerMice.size() > 1 && mvvm::utils::RandInt(0, 10) == 0)
  {
    if (mvvm::utils::RandInt(0, 1))
    {
      angle += mvvm::utils::RandDouble(0.0, 1 / 500.0);
    }
    else
    {
      angle -= mvvm::utils::RandDouble(0.0, 1 / 500.0);
    }
  }

  qreal speed = mouse_item->Property<double>(MouseItem::P_SPEED);
  speed += (-50 + mvvm::utils::RandInt(0, 100)) / 100.0;

  const qreal dx = ::sin(angle) * 10;
  mouseEyeDirection = (qAbs(dx / 5) < 1) ? 0 : dx / 5;

  auto new_coordinate = mapToParent(0, -(3 + sin(speed) * 3));
  mouse_item->SetProperty(MouseItem::P_XPOS, new_coordinate.x());
  mouse_item->SetProperty(MouseItem::P_YPOS, new_coordinate.y());
  mouse_item->SetProperty(MouseItem::P_ANGLE, angle);
  mouse_item->SetProperty(MouseItem::P_SPEED, speed);
}

}  // namespace collidingmice
