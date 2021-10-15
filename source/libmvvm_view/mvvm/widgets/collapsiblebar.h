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

#ifndef MVVM_WIDGETS_COLLAPSIBLEBAR_H
#define MVVM_WIDGETS_COLLAPSIBLEBAR_H

#include "mvvm/viewmodel_export.h"

#include <QFrame>

class QLabel;
class QString;

namespace ModelView
{
//! Horizontal collapsible bar, part of CollapsibleListWidget.
//! Intended for placement into the QSplitter, makes client widget visible/invisible on clicks.

class MVVM_VIEWMODEL_EXPORT CollapsibleBar : public QFrame
{
  Q_OBJECT

public:
  CollapsibleBar(QWidget* parent = nullptr);

  void setWidget(QWidget* widget, const QString& title);

protected:
  void mousePressEvent(QMouseEvent* event) override;

private:
  bool eventFilter(QObject* obj, QEvent* event) override;
  void updatePixmap();

  QWidget* m_controlledWidget{nullptr};
  QLabel* m_pixmapLabel{nullptr};
  QLabel* m_titleLabel{nullptr};
};

}  // namespace ModelView

#endif  // MVVM_WIDGETS_COLLAPSIBLEBAR_H
