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

#ifndef MVVM_WIDGETS_MAINBARWIDGET_H
#define MVVM_WIDGETS_MAINBARWIDGET_H

#include "mvvm/viewmodel_export.h"

#include <QFrame>
#include <map>

class QStackedWidget;
class QHBoxLayout;
class QPushButton;

namespace ModelView
{
class FancyTab;

//! Widget container with functionality similar to QTabWidget. Has large button bar on top,
//! and stacked widget at bottom.

class MVVM_VIEWMODEL_EXPORT MainBarWidget : public QFrame
{
  Q_OBJECT

public:
  MainBarWidget(QWidget* parent = nullptr);
  ~MainBarWidget();

  void addWidget(QWidget* widget, const QString& title, bool is_enabled = true);

  void setCurrentIndex(int index);

private:
  QStackedWidget* m_stackedWidget{nullptr};
  QHBoxLayout* m_labelLayout{nullptr};
  std::map<int, FancyTab*> m_indexToTab;
};

}  // namespace ModelView

#endif // MVVM_WIDGETS_MAINBARWIDGET_H
