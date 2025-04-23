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

#ifndef CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_WIDGET_H_
#define CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_WIDGET_H_

#include <QStringList>
#include <QWidget>
#include <memory>

class QLineEdit;
class QTreeView;
class QAbstractItemView;

namespace mvvm
{
class ISessionModel;
}  // namespace mvvm

namespace customeditors
{

class CustomModel;
class EditorPanel;

/**
 * @brief The EditorWidget class contains two panels to study the behavior of widgets with
 * an autocomplete feature.
 */
class EditorWidget : public QWidget
{
  Q_OBJECT

public:
  using string_list_func_t = std::function<QStringList()>;

  explicit EditorWidget(QWidget* parent_widget = nullptr);
  ~EditorWidget() override;

private:
  /**
   * @brief Creates a function which will return a list of strings with auto-complete options.
   */
  string_list_func_t CreateStringListFunc() const;
  std::unique_ptr<CustomModel> m_custom_model;
  QLineEdit* m_complete_list_edit{nullptr};
  EditorPanel* m_left_panel{nullptr};
  EditorPanel* m_right_panel{nullptr};
};

}  // namespace customeditors

#endif  // CUSTOMEDITORS_CUSTOMEDITORSCORE_EDITOR_WIDGET_H_
