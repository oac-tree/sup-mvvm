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

#ifndef MVVM_WIDGETS_FANCYTAB_H_
#define MVVM_WIDGETS_FANCYTAB_H_

#include <mvvm/viewmodel_export.h>

#include <QColor>
#include <QWidget>

class QLabel;
class QString;

namespace mvvm
{
class MVVM_VIEWMODEL_EXPORT FancyTab : public QWidget
{
  Q_OBJECT

public:
  FancyTab(const QString& title, QWidget* parent = nullptr);

  void setSelected(bool value);

signals:
  void clicked();

protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  void enterEvent(QEvent* event) override;
#else
  void enterEvent(QEnterEvent* event) override;
#endif  // MVVM_WIDGETS_FANCYTAB_H_
  void leaveEvent(QEvent* event) override;

private:
  QLabel* m_label{nullptr};
  bool m_isSelected{false};
  QColor m_widgetColor;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_FANCYTAB_H_
