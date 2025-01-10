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

#ifndef MVVM_WIDGETS_MAINBAR_WIDGET_H_
#define MVVM_WIDGETS_MAINBAR_WIDGET_H_

#include <mvvm/viewmodel_export.h>

#include <QFrame>
#include <map>

class QStackedWidget;
class QHBoxLayout;
class QPushButton;

namespace mvvm
{

class FancyTab;

/**
 * @brief The MainBarWidget class is a widget container with the functionality similar to
 * QTabWidget.
 *
 * It tries to look like horizontal list of typical Android application.
 *
 * ITEM1   ITEM2   ITEM3
 * --------=====--------
 * !                   !
 * !                   !
 * ---------------------
 * Has large bar with labels on top, and stacked widget at bottom.
 */
class MVVM_VIEWMODEL_EXPORT MainBarWidget : public QFrame
{
  Q_OBJECT

public:
  MainBarWidget(QWidget* parent_widget = nullptr);
  ~MainBarWidget();

  void AddWidget(QWidget* widget, const QString& title, bool is_enabled = true);

  void SetCurrentIndex(int index);

private:
  QStackedWidget* m_stacked_widget{nullptr};
  QHBoxLayout* m_label_layout{nullptr};
  std::map<int, FancyTab*> m_index_to_tab;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_MAINBAR_WIDGET_H_
