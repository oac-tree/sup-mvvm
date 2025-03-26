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

#include <QVBoxLayout>

namespace customeditors
{

namespace
{

/**
 * @brief Returns a function which will generate option list.
 */
mvvm::StringCompleterComboEditor::string_list_func_t CreateStringFunc()
{
  static const QStringList kOptions = {"ABC", "ABC-DEF", "ABC-DEF", "ABC-DEF-XYZ"};
  return []() { return kOptions; };
}

}  // namespace

EditorWidget::EditorWidget(QWidget *parent_widget)
    : QWidget(parent_widget)
    , m_combo_editor(new mvvm::StringCompleterComboEditor(CreateStringFunc()))
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_combo_editor);
}

}  // namespace customeditors
