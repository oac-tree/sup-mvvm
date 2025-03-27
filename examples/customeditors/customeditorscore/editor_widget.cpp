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

#include "editor_widget.h"

#include <mvvm/editors/string_completer_combo_editor.h>
#include <mvvm/editors/string_completer_editor.h>

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTreeView>
#include <QVBoxLayout>

namespace customeditors
{

namespace
{

const QStringList kInitAutocompleteOptions = {"ABC", "ABC-DEF", "ABC-DEF", "ABC-DEF-XYZ"};

QStringList GetListFromString(const QString& str)
{
  QStringList splitted = str.split(",");
  QStringList result;
  std::transform(splitted.begin(), splitted.end(), std::back_inserter(result),
                 [](auto element) { return element.trimmed(); });
  return result;
}

}  // namespace

EditorWidget::EditorWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_complete_list_edit(new QLineEdit)
    , m_left_tree_view(new QTreeView)
    , m_right_tree_view(new QTreeView)
    , m_grid_layout(new QGridLayout)
{
  auto layout = new QVBoxLayout(this);

  m_complete_list_edit->setText(kInitAutocompleteOptions.join(", "));
  m_combo_editor = new mvvm::StringCompleterComboEditor(CreateStringListFunc());
  m_line_editor = new mvvm::StringCompleterEditor(CreateStringListFunc());

  m_grid_layout->addWidget(new QLabel("Autocomplete options"), 0, 0);
  m_grid_layout->addWidget(m_complete_list_edit, 0, 1);

  m_grid_layout->addWidget(new QLabel("String autocomplete"), 1, 0);
  m_grid_layout->addWidget(m_line_editor, 1, 1);

  m_grid_layout->addWidget(new QLabel("Combo autocomplete"), 2, 0);
  m_grid_layout->addWidget(m_combo_editor, 2, 1);

  auto horizontal_layout = new QHBoxLayout;
  horizontal_layout->addWidget(m_left_tree_view);
  horizontal_layout->addWidget(m_right_tree_view);

  layout->addLayout(m_grid_layout);
  layout->addLayout(horizontal_layout);
}

EditorWidget::string_list_func_t EditorWidget::CreateStringListFunc() const
{
  return [this]() { return GetListFromString(m_complete_list_edit->text()); };
}

}  // namespace customeditors
