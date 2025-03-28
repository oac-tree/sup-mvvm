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
#include <QLineEdit>
#include <QTreeView>
#include <QVBoxLayout>

namespace customeditors
{

namespace
{
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

EditorPanel::EditorPanel(CustomModel* model, const std::function<QStringList()>& string_list_func,
                         QWidget* parent_widget)
    : QWidget(parent_widget)
    , m_model(model)
    , m_string_list_func(string_list_func)
    , m_tree_view(new QTreeView)
    , m_grid_layout(new QGridLayout)
{
  auto layout = new QVBoxLayout(this);

  m_combo_editor = new mvvm::StringCompleterComboEditor(m_string_list_func);
  m_line_editor = new mvvm::StringCompleterEditor(m_string_list_func);

  m_grid_layout->addWidget(new QLabel("String autocomplete"), 1, 0);
  m_grid_layout->addWidget(m_line_editor, 1, 1);

  m_grid_layout->addWidget(new QLabel("Combo autocomplete"), 2, 0);
  m_grid_layout->addWidget(m_combo_editor, 2, 1);

  layout->addLayout(m_grid_layout);
  layout->addWidget(m_tree_view);

  SetupTreeViews();
}

EditorPanel::~EditorPanel() = default;

std::unique_ptr<mvvm::ItemViewComponentProvider> EditorPanel::CreateCustomProvider(
    QAbstractItemView* view)
{
  auto factory = std::make_unique<CustomEditorFactory>(m_string_list_func);
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
