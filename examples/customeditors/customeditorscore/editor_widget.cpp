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

#include "custom_model.h"
#include "editor_panel.h"

#include <mvvm/editors/string_completer_combo_editor.h>
#include <mvvm/editors/string_completer_editor.h>

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

/**
 * @brief Returns list from comma separated string.
 */
QStringList GetListFromString(const QString& str)
{
  const QStringList splitted = str.split(",");
  QStringList result;
  std::transform(splitted.begin(), splitted.end(), std::back_inserter(result),
                 [](auto& element) { return element.trimmed(); });
  return result;
}

}  // namespace

EditorWidget::EditorWidget(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_custom_model(std::make_unique<CustomModel>())
    , m_complete_list_edit(new QLineEdit)
    , m_left_panel(new EditorPanel(m_custom_model.get(), CreateStringListFunc()))
    , m_right_panel(new EditorPanel(m_custom_model.get(), CreateStringListFunc()))
{
  auto layout = new QVBoxLayout(this);

  auto horizontal_layout0 = new QHBoxLayout;
  horizontal_layout0->addWidget(new QLabel("Autocomplete options"));
  horizontal_layout0->addWidget(m_complete_list_edit);
  m_complete_list_edit->setText(kInitAutocompleteOptions.join(", "));

  auto horizontal_layout1 = new QHBoxLayout;
  horizontal_layout1->addWidget(m_left_panel);
  horizontal_layout1->addWidget(m_right_panel);

  layout->addLayout(horizontal_layout0);
  layout->addLayout(horizontal_layout1);

  connect(m_left_panel->GetLineEdit(), &QLineEdit::editingFinished, m_right_panel->GetLineEdit(),
          [this]() { m_right_panel->GetLineEdit()->setText(m_left_panel->GetLineEdit()->text()); });

  connect(m_left_panel->GetLineCompleterEditor(), &mvvm::StringCompleterEditor::valueChanged,
          m_right_panel->GetLineCompleterEditor(), &mvvm::StringCompleterEditor::setValue);

  connect(m_left_panel->GetComboCompleterEditor(), &mvvm::StringCompleterComboEditor::valueChanged,
          m_right_panel->GetComboCompleterEditor(), &mvvm::StringCompleterComboEditor::setValue);
}

EditorWidget::~EditorWidget() = default;

EditorWidget::string_list_func_t EditorWidget::CreateStringListFunc() const
{
  return [this]() { return GetListFromString(m_complete_list_edit->text()); };
}

}  // namespace customeditors
