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

#ifndef CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_PANEL_H_
#define CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_PANEL_H_

#include <QStringList>
#include <QWidget>
#include <memory>

class QGridLayout;
class QTreeView;
class QAbstractItemView;

namespace mvvm
{
class ISessionModel;
class StringCompleterComboEditor;
class StringCompleterEditor;
class AllItemsViewModel;
class ItemViewComponentProvider;
}  // namespace mvvm

namespace customeditors
{

class CustomModel;

/**
 * @brief The EditorWidget class contains two types of widgets with auto-completing feature.
 *
 * Widgets are embedded either in the main layout, or generated in the fly in cells of Qt tree.
 */
class EditorPanel : public QWidget
{
  Q_OBJECT

public:
  using completer_list_func_t = std::function<QStringList()>;

  explicit EditorPanel(CustomModel* model, const std::function<QStringList()>& string_list_func,
                       QWidget* parent_widget = nullptr);
  ~EditorPanel() override;

  void SetLineEditValue(const QString& str);

  void SetComboEditorValue(const QString& str);

signals:
  void LineEditValueChanged(const QString& str);
  void ComboEditorValueChanged(const QString& str);

private:
  /**
   * @brief Creates a provider to serve given view.
   */
  std::unique_ptr<mvvm::ItemViewComponentProvider> CreateCustomProvider(QAbstractItemView* view);

  void SetupTreeViews();

  CustomModel* m_model{nullptr};
  completer_list_func_t m_completer_list_func;

  mvvm::StringCompleterEditor* m_line_editor{nullptr};
  mvvm::StringCompleterComboEditor* m_combo_editor{nullptr};
  QTreeView* m_tree_view{nullptr};
  QGridLayout* m_grid_layout{nullptr};

  std::unique_ptr<mvvm::ItemViewComponentProvider> m_tree_provider;
};

}  // namespace customeditors

#endif  // CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_PANEL_H_
