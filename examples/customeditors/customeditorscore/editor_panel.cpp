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

#include "editor_panel.h"

#include "custom_editor_factory.h"
#include "custom_model.h"

#include <mvvm/editors/string_completer_combo_editor.h>
#include <mvvm/editors/string_completer_editor.h>
#include <mvvm/viewmodel/all_items_viewmodel.h>
#include <mvvm/views/component_provider_helper.h>
#include <mvvm/views/default_cell_decorator.h>
#include <mvvm/views/viewmodel_delegate.h>

#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QTreeView>
#include <QVBoxLayout>

namespace customeditors
{

EditorPanel::EditorPanel(CustomModel* model, const std::function<QStringList()>& string_list_func,
                         QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_model(model)
    , m_completer_list_func(string_list_func)
    , m_line_editor(new mvvm::StringCompleterEditor(m_completer_list_func))
    , m_combo_editor(new mvvm::StringCompleterComboEditor(m_completer_list_func))
    , m_tree_view(new QTreeView)
    , m_grid_layout(new QGridLayout)
{
  auto layout = new QVBoxLayout(this);

  m_grid_layout->addWidget(new QLabel("String completer"), 1, 0);
  m_grid_layout->addWidget(m_line_editor, 1, 1);

  m_grid_layout->addWidget(new QLabel("Combo completer"), 2, 0);
  m_grid_layout->addWidget(m_combo_editor, 2, 1);

  layout->addLayout(m_grid_layout);
  layout->addWidget(m_tree_view);

  SetupTreeViews();

  // connect(m_line_editor, &mvvm::StringCompleterEditor::valueChanged, this,
  //         [this](auto variant) { emit LineEditValueChanged(variant.toString()); });
  // connect(m_combo_editor, &mvvm::StringCompleterComboEditor::valueChanged, this,
  //         [this](auto variant) { emit ComboEditorValueChanged(variant.toString()); });
}

void EditorPanel::SetLineEditValue(const QString& str)
{
  m_line_editor->setValue(str);
}

void EditorPanel::SetComboEditorValue(const QString& str)
{
  m_combo_editor->setValue(str);
}

EditorPanel::~EditorPanel() = default;

std::unique_ptr<mvvm::ItemViewComponentProvider> EditorPanel::CreateCustomProvider(
    QAbstractItemView* view)
{
  auto factory = std::make_unique<CustomEditorFactory>(m_completer_list_func);
  auto decorator = std::make_unique<mvvm::DefaultCellDecorator>();
  auto delegate =
      std::make_unique<mvvm::ViewModelDelegate>(std::move(factory), std::move(decorator));
  auto viewmodel = std::make_unique<mvvm::AllItemsViewModel>(m_model);

  return std::make_unique<mvvm::ItemViewComponentProvider>(std::move(delegate),
                                                           std::move(viewmodel), view);
}

void EditorPanel::SetupTreeViews()
{
  m_tree_provider = CreateCustomProvider(m_tree_view);

  m_tree_view->expandAll();

  m_tree_view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

}  // namespace customeditors
