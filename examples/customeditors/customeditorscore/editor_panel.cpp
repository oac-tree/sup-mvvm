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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "editor_panel.h"

#include "custom_editor_factory.h"
#include "custom_model.h"

#include <mvvm/editors/string_completer_combo_editor.h>
#include <mvvm/editors/string_completer_editor.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>
#include <mvvm/views/default_cell_decorator.h>
#include <mvvm/views/viewmodel_component_builder.h>
#include <mvvm/views/viewmodel_delegate.h>

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QTreeView>
#include <QVBoxLayout>

namespace customeditors
{

EditorPanel::EditorPanel(CustomModel* model, const std::function<QStringList()>& string_list_func,
                         QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_model(model)
    , m_completer_list_func(string_list_func)
    , m_line_edit(new QLineEdit)
    , m_line_completer_editor(new mvvm::StringCompleterEditor(m_completer_list_func))
    , m_combo_completer_editor(new mvvm::StringCompleterComboEditor(m_completer_list_func))
    , m_tree_view(new QTreeView)
    , m_grid_layout(new QGridLayout)
{
  auto layout = new QVBoxLayout(this);

  m_grid_layout->addWidget(new QLabel("Line edit"), 0, 0);
  m_grid_layout->addWidget(m_line_edit, 0, 1);

  m_grid_layout->addWidget(new QLabel("String completer"), 2, 0);
  m_grid_layout->addWidget(m_line_completer_editor, 2, 1);

  m_grid_layout->addWidget(new QLabel("Combo completer"), 3, 0);
  m_grid_layout->addWidget(m_combo_completer_editor, 3, 1);

  layout->addLayout(m_grid_layout);
  layout->addWidget(m_tree_view);

  SetupTreeViews();
}

QLineEdit* EditorPanel::GetLineEdit()
{
  return m_line_edit;
}

mvvm::StringCompleterEditor* EditorPanel::GetLineCompleterEditor()
{
  return m_line_completer_editor;
}

mvvm::StringCompleterComboEditor* EditorPanel::GetComboCompleterEditor()
{
  return m_combo_completer_editor;
}

EditorPanel::~EditorPanel() = default;

std::unique_ptr<mvvm::ItemViewComponentProvider> EditorPanel::CreateCustomProvider(
    QAbstractItemView* view)
{
  // magic: thanks to "operator std::unique_ptr<ItemViewComponentProvider>()"
  return mvvm::CreateProvider()
      .ViewModel<mvvm::AllItemsViewModel>(m_model)
      .View(view)
      .Factory<CustomEditorFactory>(m_completer_list_func)
      .Decorator<mvvm::DefaultCellDecorator>();
}

void EditorPanel::SetupTreeViews()
{
  m_tree_provider = CreateCustomProvider(m_tree_view);

  m_tree_view->expandAll();

  m_tree_view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

}  // namespace customeditors
