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

#ifndef MVVM_WIDGETS_MAINVERTICALBARWIDGET_H
#define MVVM_WIDGETS_MAINVERTICALBARWIDGET_H

#include "mvvm/viewmodel_export.h"

#include <QFrame>
#include <map>

class QStackedWidget;
class QVBoxLayout;
class QPushButton;
class QIcon;
class QButtonGroup;
class QToolButton;

namespace ModelView
{
\
//! Widget container with functionality similar to QTabWidget.
//! Has group of buttons on the left and stacked widget on the right.

class MVVM_VIEWMODEL_EXPORT MainVerticalBarWidget : public QFrame
{
  Q_OBJECT

public:
  MainVerticalBarWidget(QWidget* parent = nullptr);
  ~MainVerticalBarWidget();

  void addWidget(QWidget* widget, const QString& title, const QIcon& icon);

  void setCurrentIndex(int index);

private:
  QToolButton* CreateViewSelectionButton();
  void UpdateViewSelectionButtonsGeometry();

  QStackedWidget* m_stacked_widget{nullptr};
  QVBoxLayout* m_button_layout{nullptr};
  QButtonGroup* m_button_group{nullptr};
};

}  // namespace ModelView

#endif // MVVM_WIDGETS_MAINVERTICALBARWIDGET_H
