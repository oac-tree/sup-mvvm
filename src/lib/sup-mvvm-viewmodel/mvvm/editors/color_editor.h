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

#ifndef MVVM_EDITORS_COLOR_EDITOR_H_
#define MVVM_EDITORS_COLOR_EDITOR_H_

#include <mvvm/editors/custom_editor.h>

class QLabel;

namespace mvvm
{

class LostFocusFilter;

//! Custom editor for QVariant based on QString that contains color name.

class MVVM_VIEWMODEL_EXPORT ColorEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit ColorEditor(QWidget* parent_widget = nullptr);

protected:
  void mousePressEvent(QMouseEvent* event) override;

private:
  QColor GetCurrentColor() const;

  void UpdateComponents() override;
  QLabel* m_text_label{nullptr};
  QLabel* m_pixmap_label{nullptr};
  LostFocusFilter* m_focus_filter;
};

}  // namespace mvvm

#endif  // MVVM_EDITORS_COLOR_EDITOR_H_
