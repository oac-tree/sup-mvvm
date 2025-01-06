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

#ifndef MVVM_EDITORS_CUSTOM_EVENT_FILTERS_H_
#define MVVM_EDITORS_CUSTOM_EVENT_FILTERS_H_

#include <mvvm/viewmodel_export.h>

#include <QObject>

namespace mvvm
{

//! Filter out space bar key events, which is special case for dialog windows.

class SpaceKeyEater : public QObject
{
  Q_OBJECT

public:
  explicit SpaceKeyEater(QObject* parent = nullptr);

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;
};

//! Event filter to prevent loss of the focus.
//! Can be used in the context of QTreeView and similar widgets to call external editor. Such an
//! editor is created by clicking on a cell of a tree and it appears as  modal window on top of a
//! tree.

class MVVM_VIEWMODEL_EXPORT LostFocusFilter : public QObject
{
  Q_OBJECT

public:
  explicit LostFocusFilter(QObject* parent = nullptr);

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;
};

//! Event filter to install on combo boxes and spin boxes to ignore wheel events during scrolling.
//! Helpful than the spin box is a child of some larger scroll area.

class MVVM_VIEWMODEL_EXPORT WheelEventFilter : public QObject
{
  Q_OBJECT

public:
  explicit WheelEventFilter(QObject* parent = nullptr);

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;
};

//! Event filter for global tracking of shortcodes.

class ShortcodeFilter : public QObject
{
  Q_OBJECT

public:
  explicit ShortcodeFilter(const QString& shortcode, QObject* parent = nullptr);

signals:
  void found();

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  QString m_shortcode;
  int m_index;
};

//! Propagate tab events from focusProxy to parent.

class TabFromFocusProxy : public QObject {
  Q_OBJECT

public:
  TabFromFocusProxy(QWidget* parent = nullptr);

protected:
  bool eventFilter(QObject* obj, QEvent* event);

public:
  QWidget* m_parent{nullptr};
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_CUSTOM_EVENT_FILTERS_H_
