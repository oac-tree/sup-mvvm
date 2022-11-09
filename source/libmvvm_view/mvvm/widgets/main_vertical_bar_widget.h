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

#ifndef MVVM_WIDGETS_MAIN_VERTICAL_BAR_WIDGET_H_
#define MVVM_WIDGETS_MAIN_VERTICAL_BAR_WIDGET_H_

#include <mvvm/viewmodel_export.h>

#include <QFrame>
#include <QColor>
#include <map>

class QStackedWidget;
class QVBoxLayout;
class QPushButton;
class QIcon;
class QButtonGroup;
class QToolButton;
class QStatusBar;

namespace mvvm
{

//! Widget container with functionality similar to QTabWidget.
//! Has group of buttons on the left and stacked widget on the right.

class MVVM_VIEWMODEL_EXPORT MainVerticalBarWidget : public QFrame
{
  Q_OBJECT

public:
  MainVerticalBarWidget(QWidget* parent = nullptr);
  ~MainVerticalBarWidget();

  void AddWidget(QWidget* widget, const QString& title, const QIcon& icon);

  void SetCurrentIndex(int index);

  void AddSpacer();

  void SetBaseColor(const QColor& color);

  QStatusBar* GetStatusBar();

private:
  QToolButton* CreateViewSelectionButton();
  void UpdateViewSelectionButtonsGeometry();

  QStackedWidget* m_stacked_widget{nullptr};
  QVBoxLayout* m_button_layout{nullptr};
  QButtonGroup* m_button_group{nullptr};
  QToolButton* m_filler_button{nullptr};
  QStatusBar* m_status_bar{nullptr};

  QColor m_base_color;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_MAIN_VERTICAL_BAR_WIDGET_H_
