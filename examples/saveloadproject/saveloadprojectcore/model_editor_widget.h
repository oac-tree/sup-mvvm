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

#ifndef SAVELOADPROJECT_SAVELOADPROJECTCORE_MODEL_EDITOR_WIDGET_H_
#define SAVELOADPROJECT_SAVELOADPROJECTCORE_MODEL_EDITOR_WIDGET_H_

#include <QWidget>

class QToolBar;
class QAction;

namespace saveloadproject
{

class SampleModel;
class ContainerEditorWidget;
class RecentProjectWidget;

class ModelEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ModelEditorWidget(QWidget* parent = nullptr);
  ~ModelEditorWidget() override;

  void SetModel(SampleModel* model);

  void UpdateCurrentProjectInfo(const std::string& path, bool is_modified);

  void UpdateRecentProjectList(const std::vector<std::string>& recent_list);

signals:
  void projectSelected(const QString& project_path);

private:
  RecentProjectWidget* m_recent_project_widget{nullptr};
  ContainerEditorWidget* m_right_widget{nullptr};
  SampleModel* m_model{nullptr};
};

}  // namespace saveloadproject

#endif  // SAVELOADPROJECT_SAVELOADPROJECTCORE_MODEL_EDITOR_WIDGET_H_
