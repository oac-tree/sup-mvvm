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

#include "custom_event_filters.h"

#include <QAbstractSpinBox>
#include <QApplication>
#include <QComboBox>
#include <QEvent>
#include <QKeyEvent>

namespace mvvm
{

// -------------------------------------------------------------------------------------------------
// SpaceKeyEater
// -------------------------------------------------------------------------------------------------

SpaceKeyEater::SpaceKeyEater(QObject* parent_object) : QObject(parent_object) {}

bool SpaceKeyEater::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::KeyPress)
  {
    auto key_event = dynamic_cast<QKeyEvent*>(event);

    if (key_event->key() == Qt::Key_Space)
    {
      return true; /* Always accept space bar */
    }
  }

  return QObject::eventFilter(obj, event);
}

// -------------------------------------------------------------------------------------------------
// LostFocusFilter
// -------------------------------------------------------------------------------------------------

LostFocusFilter::LostFocusFilter(QObject* parent_object) : QObject(parent_object) {}

bool LostFocusFilter::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::FocusOut)
  {
    return true;
  }

  return QObject::eventFilter(obj, event);
}

// -------------------------------------------------------------------------------------------------
// WheelEventFilter
// -------------------------------------------------------------------------------------------------

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

// -------------------------------------------------------------------------------------------------
// ShortcodeFilter
// -------------------------------------------------------------------------------------------------

ShortcodeFilter::ShortcodeFilter(const QString& shortcode, QObject* parent_object)
    : QObject(parent_object), m_shortcode(shortcode), m_index(0)
{
}

bool ShortcodeFilter::eventFilter(QObject* obj, QEvent* event)
{
  Q_UNUSED(obj);
  if (event->type() == QEvent::KeyPress)
  {
    auto keyEvent = dynamic_cast<QKeyEvent*>(event);
    if (m_shortcode.at(m_index) == keyEvent->text())
    {
      m_index++;
      if (m_index == m_shortcode.length())
      {
        emit found();
        m_index = 0;
      }
    }
    else
    {
      int right = m_index;
      while (m_index > 0)
      {
        if (m_shortcode.at(m_index - 1) == keyEvent->text()
            && m_shortcode.left(m_index - 1) == m_shortcode.mid(right - m_index + 1, m_index - 1))
        {
          break;
        }
        m_index--;
      }
    }
  }
  return false;
}

// -------------------------------------------------------------------------------------------------
// TabFromFocusProxy
// -------------------------------------------------------------------------------------------------

TabFromFocusProxy::TabFromFocusProxy(QWidget* parent_widget) : QObject(parent_widget), m_parent(parent_widget)
{
  if (parent_widget->focusProxy())
  {
    parent_widget->focusProxy()->installEventFilter(this);
  }
}

bool TabFromFocusProxy::eventFilter(QObject* obj, QEvent* event)
{
  // Passing focus-related events from child widget (e.g. QSpinBox) to parent (e.g. IntEditor)
  // to trigger QTreeView delegate's mechanism to switch editors on "tab" press key.
  // https://stackoverflow.com/questions/12145522/why-pressing-of-tab-key-emits-only-qeventshortcutoverride-event

  if (event->type() == QEvent::KeyPress)
  {
    auto key_event = static_cast<QKeyEvent*>(event);
    if (key_event->key() == Qt::Key_Tab || key_event->key() == Qt::Key_Backtab)
    {
      // we are posting event as if m_parent had "tab" key
      QApplication::postEvent(
          m_parent, new QKeyEvent(key_event->type(), key_event->key(), key_event->modifiers()));

      // but still let the origin (QSpinBox) to process it
      return false;  // process
    }
  }

  else if (event->type() == QEvent::FocusOut)
  {
    auto focus_event = static_cast<QFocusEvent*>(event);
    QApplication::postEvent(this, new QFocusEvent(focus_event->type(), focus_event->reason()));

    // Don't filter because focus can be changed internally in editor
    return false;
  }

  return QObject::eventFilter(obj, event);
}

}  // namespace mvvm
