/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/editors/custom_event_filters.h"

#include <QAbstractSpinBox>
#include <QComboBox>
#include <QEvent>
#include <QKeyEvent>

namespace mvvm
{

SpaceKeyEater::SpaceKeyEater(QObject* parent) : QObject(parent) {}

bool SpaceKeyEater::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::KeyPress)
  {
    auto key_event = dynamic_cast<QKeyEvent*>(event);
    bool res = QObject::eventFilter(obj, event);

    if (key_event->key() == Qt::Key_Space)
    {
      return true; /* Always accept space bar */
    }
  }

  return QObject::eventFilter(obj, event);
}

// ----------------------------------------------------------------------------

LostFocusFilter::LostFocusFilter(QObject* parent) : QObject(parent) {}

bool LostFocusFilter::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::FocusOut)
  {
    return true;
  }

  return QObject::eventFilter(obj, event);
}

// ----------------------------------------------------------------------------

WheelEventFilter::WheelEventFilter(QObject* parent) : QObject(parent) {}

bool WheelEventFilter::eventFilter(QObject* obj, QEvent* event)
{
  if (auto spinBox = qobject_cast<QAbstractSpinBox*>(obj); spinBox)
  {
    if (event->type() == QEvent::Wheel)
    {
      if (spinBox->focusPolicy() == Qt::WheelFocus)
      {
        event->accept();
        return false;
      }
      else
      {
        event->ignore();
        return true;
      }
    }
    else if (event->type() == QEvent::FocusIn)
    {
      spinBox->setFocusPolicy(Qt::WheelFocus);
    }
    else if (event->type() == QEvent::FocusOut)
    {
      spinBox->setFocusPolicy(Qt::StrongFocus);
    }
  }
  else if (auto comboBox = qobject_cast<QComboBox*>(obj); comboBox)
  {
    if (event->type() == QEvent::Wheel)
    {
      event->ignore();
      return true;
    }
    else
    {
      event->accept();
      return false;
    }
  }
  return QObject::eventFilter(obj, event);
}

}  // namespace mvvm
