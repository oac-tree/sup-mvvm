/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_WIDGETS_COLLAPSIBLE_TOOLBAR_H_
#define MVVM_WIDGETS_COLLAPSIBLE_TOOLBAR_H_

#include <QFrame>
#include <QList>

class QToolBar;
class QLabel;
class QToolButton;
class QAction;

namespace mvvm
{

//! Instrument tool bar with possibility to collapse/expand attached widget.
//! Toolbar buttons are shown only when expanded.
//! Looks like this: > LABEL BUTTON BUTTON BUTTON

//! This toolbar is intended to function in splitters together with CollapsibleWidgets.

class CollapsibleToolBar : public QFrame
{
  Q_OBJECT

public:
  explicit CollapsibleToolBar(QWidget* parent = nullptr);

  void SetText(const QString& text, const QString& text_tooltip = {});

  void SetControlledWidget(QWidget* widget);

  void AddWidget(QWidget* widget);
  void AddWidgets(const QList<QWidget*>& widgets);

  void AddAction(QAction* action);
  void AddActions(const QList<QAction*> actions);

  QToolBar* GetToolBar();

private:
  void UpdateToolBar();
  void UpdateIcon();
  void InsertStrech();

  QToolBar* m_tool_bar{nullptr};
  QToolButton* m_expand_button{nullptr};
  QLabel* m_label{nullptr};
  bool m_expanded{true};
  QWidget* m_controlled_widget{nullptr};
  QList<QAction*> m_toolbar_actions;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_COLLAPSIBLE_TOOLBAR_H_
