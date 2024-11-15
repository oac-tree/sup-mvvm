// ************************************************************************** //
//
/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SAVELOADPROJECT_SAVELOADPROJECTCORE_RECENT_PROJECT_WIDGET_H_
#define SAVELOADPROJECT_SAVELOADPROJECTCORE_RECENT_PROJECT_WIDGET_H_

#include <QWidget>
#include <vector>

class QBoxLayout;

namespace saveloadproject
{

class ProjectPaneWidget;

/**
 * @brief The RecentProjectWidget class shows row of panels representing available projects.
 */
class RecentProjectWidget : public QWidget
{
  Q_OBJECT

public:
  explicit RecentProjectWidget(QWidget* parent = nullptr);

  /**
   * @brief Set a project label on the project panel, dedicated to displaying the currently opened
   * project.
   */
  void UpdateCurrentProjectInfo(const QString& path, bool is_modified);

  /**
   * @brief Update names of recently opened projects.
   */
  void UpdateRecentProjectsList(const QStringList& paths);

  QSize sizeHint() const override;

  QSize minimumSizeHint() const override;

signals:
  void projectSelected(const QString& project_dir);

private:
  QBoxLayout* CreateCurrentProjectLayout() const;
  QBoxLayout* CreateRecentProjectLayout();
  QWidget* CreateRecentProjectScrollArea();

  ProjectPaneWidget* m_current_project_pane{nullptr};
  std::vector<ProjectPaneWidget*> m_recent_project_panes;
};

}  // namespace saveloadproject

#endif  // SAVELOADPROJECT_SAVELOADPROJECTCORE_RECENT_PROJECT_WIDGET_H_
