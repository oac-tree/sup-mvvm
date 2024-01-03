/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_WIDGETS_STATUS_LABEL_H_
#define MVVM_WIDGETS_STATUS_LABEL_H_

#include <mvvm/view_export.h>

#include <QFrame>

class QPaintEvent;

namespace mvvm
{

//! Shows a single line of text on a white background.
//! Opposite to QLabel, doesn't trigger layout resize, being happy with place it has. If text string
//! is too long for current size, it will be clipped.

class MVVM_VIEW_EXPORT StatusLabel : public QFrame
{
  Q_OBJECT

public:
  explicit StatusLabel(QWidget* parent = nullptr);

  void setText(const QString& text);
  void setFont(const QFont& font);
  void setPointSize(int pointSize);
  void setAlignment(Qt::Alignment);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  QString m_text;
  Qt::Alignment m_alignment;
  QFont m_font;
};

}  // namespace mvvm

#endif  // MVVM_WIDGETS_STATUS_LABEL_H_
