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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_WIDGETS_MAIN_VERTICAL_BAR_WIDGET_H_
#define MVVM_WIDGETS_MAIN_VERTICAL_BAR_WIDGET_H_

#include <mvvm/viewmodel_export.h>

#include <QColor>
#include <QFrame>
#include <memory>

class QStackedWidget;
class QVBoxLayout;
class QPushButton;
class QIcon;
class QButtonGroup;
class QToolButton;
class QStatusBar;

namespace mvvm
{

/**
 * @brief The MainVerticalBarWidget class represents a widget container with large buttons on the
 * left and widget stack on the right.
 *
 * It has functionality similar to QTabWidget. The control bar tries to\ mimic main control bar of
 * Qt creator, or VSCode applications.
 */
class MVVM_VIEWMODEL_EXPORT MainVerticalBarWidget : public QFrame
{
  Q_OBJECT

public:
  MainVerticalBarWidget(QWidget* parent_widget = nullptr);
  ~MainVerticalBarWidget();

  /**
   * @brief Adds widget to the toolbar.
   *
   *  The ownerhip of the widget will be taken.
   *
   * @param widget Users widget.
   * @param title A title will be show under the button's icon.
   * @param icon The icon of the button.
   */
  void AddWidget(QWidget* widget, const QString& title, const QIcon& icon);

  /**
   * @brief Set widget corresponding to a given index as visible.
   */
  void SetCurrentIndex(int index);

  /**
   * @brief Adds spacer after the last widget.
   */
  void AddSpacer();

  /**
   * @brief Sets the base color of toolbar.
   */
  void SetBaseColor(const QColor& color);

  /**
   * @brief Returns status bar.
   *
   * Initially it is invisible. If the user wants to use it as the main status bar of QMainWindow,
   * he has to change visibility and add it to the main window.
   */
  QStatusBar* GetStatusBar();

private:
  std::unique_ptr<QToolButton> CreateControlButton();
  void UpdateButtonLayout();

  QStackedWidget* m_stacked_widget{nullptr};
  QVBoxLayout* m_button_layout{nullptr};
  QButtonGroup* m_button_group{nullptr};
  QToolButton* m_filler_button{nullptr};
  QStatusBar* m_status_bar{nullptr};
  QColor m_base_color;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_MAIN_VERTICAL_BAR_WIDGET_H_
