// ************************************************************************** //
//
//  Model-view-view-model framework for large GUI applications
//
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @authors   see AUTHORS
//
// ************************************************************************** //

#ifndef MVVM_EDITORS_COLOREDITOR_H
#define MVVM_EDITORS_COLOREDITOR_H

#include "mvvm/editors/customeditor.h"

class QLabel;

namespace mvvm
{

class LostFocusFilter;

//! Custom editor for QVariant based on QString that contains color name.

class MVVM_VIEWMODEL_EXPORT ColorEditor : public CustomEditor
{
  Q_OBJECT

public:
  explicit ColorEditor(QWidget* parent = nullptr);

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

#endif  // MVVM_EDITORS_COLOREDITOR_H
