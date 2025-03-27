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

#ifndef CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_WIDGET_H_
#define CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_WIDGET_H_

#include <QWidget>
#include <QStringList>

class QGridLayout;
class QLineEdit;
class QTreeView;

namespace mvvm
{
class StringCompleterComboEditor;
class StringCompleterEditor;
}  // namespace mvvm

namespace customeditors
{

class EditorWidget : public QWidget
{
  Q_OBJECT

public:
  using string_list_func_t = std::function<QStringList()>;

  explicit EditorWidget(QWidget* parent_widget = nullptr);

private:
  /**
   * @brief Creates a function which will return a list of strings with auto-complete options.
   */
  string_list_func_t CreateStringListFunc() const;

  QLineEdit* m_complete_list_edit{nullptr};
  mvvm::StringCompleterComboEditor* m_combo_editor{nullptr};
  mvvm::StringCompleterEditor* m_line_editor{nullptr};
  QTreeView* m_left_tree_view{nullptr};
  QTreeView* m_right_tree_view{nullptr};
  QGridLayout* m_grid_layout{nullptr};
};

}  // namespace customeditors

#endif  // CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_WIDGET_H_
