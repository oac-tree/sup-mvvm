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

#ifndef MVVM_WIDGETS_COLLAPSIBLELISTWIDGET_H
#define MVVM_WIDGETS_COLLAPSIBLELISTWIDGET_H

#include "mvvm/viewmodel_export.h"

#include <QWidget>

class QSplitter;
class QString;

namespace ModelView
{
//! Vertical widget with column of panels displayed one under another.
//! Each panel contains user widget and can be collapsed/expanded. When expanded,
//! the place occupied by the panel can be changed by draging a splitter.

class MVVM_VIEWMODEL_EXPORT CollapsibleListWidget : public QWidget
{
  Q_OBJECT

public:
  CollapsibleListWidget(QWidget* parent = nullptr);

  void addWidget(QWidget* widget, const QString& title, bool collapsed = false);

private:
  QSplitter* m_splitter{nullptr};
};

}  // namespace ModelView

#endif // MVVM_WIDGETS_COLLAPSIBLELISTWIDGET_H
