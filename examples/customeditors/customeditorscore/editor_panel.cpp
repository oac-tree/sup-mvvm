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

#include <QDebug>
#include <QHBoxLayout>
#include <QHeaderView>
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

EditorPanel::EditorPanel(QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_complete_list_edit(new QLineEdit)
    , m_left_tree_view(new QTreeView)
    , m_right_tree_view(new QTreeView)
    , m_grid_layout(new QGridLayout)
    , m_custom_model(std::make_unique<CustomModel>())
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

  SetupTreeViews();
}

EditorPanel::~EditorPanel() = default;

std::unique_ptr<mvvm::ItemViewComponentProvider> EditorPanel::CreateCustomProvider(
    QAbstractItemView* view)
{
  auto factory = std::make_unique<CustomEditorFactory>(CreateStringListFunc());
  auto decorator = std::make_unique<mvvm::DefaultCellDecorator>();
  auto delegate =
      std::make_unique<mvvm::ViewModelDelegate>(std::move(factory), std::move(decorator));
  auto viewmodel = std::make_unique<mvvm::AllItemsViewModel>(m_custom_model.get());

  return std::make_unique<mvvm::ItemViewComponentProvider>(std::move(delegate),
                                                           std::move(viewmodel), view);
}

void EditorPanel::SetupTreeViews()
{
  m_left_provider = CreateCustomProvider(m_left_tree_view);
  m_right_provider = CreateCustomProvider(m_right_tree_view);

  m_left_tree_view->expandAll();
  m_right_tree_view->expandAll();

  m_left_tree_view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_right_tree_view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

EditorPanel::string_list_func_t EditorPanel::CreateStringListFunc() const
{
  return [this]()
  {
    qDebug() << "Generating list";
    auto result = GetListFromString(m_complete_list_edit->text());
    qDebug() << m_complete_list_edit->text() << result;
    return result;
  };
}

}  // namespace customeditors
