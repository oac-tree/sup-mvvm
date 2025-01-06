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

#ifndef SAVELOADPROJECT_SAVELOADPROJECTCORE_PROJECT_PANE_WIDGET_H_
#define SAVELOADPROJECT_SAVELOADPROJECTCORE_PROJECT_PANE_WIDGET_H_

#include <QWidget>

class QLabel;

namespace saveloadproject
{

/**
 * @brief The ProjectPaneWidget class is a panel with labels to holt project name and project
 * directory.
 */
class ProjectPaneWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ProjectPaneWidget(QWidget* parent = nullptr);

  /**
   * @brief Set panels' labels according to project information.
   */
  void SetProjectInfo(const QString& path, bool is_modified = false);

  /**
   * @brief Clears labels and make widget inactive.
   *
   * Inactive widget doesnt' send signals when user click on it.
   */
  void Clear();

  /**
   * @brief Sets active flag to the given value.
   *
   * 'False' means that the widget only shows the project title, but doesn't react on mouse clicks
   * and doesn't change the background on mouse hover events.
   */
  void SetActive(bool value);

signals:
  void projectSelected(const QString& project_path);

protected:
  void paintEvent(QPaintEvent* event) override;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  void enterEvent(QEvent* event) override;
#else
  void enterEvent(QEnterEvent* event) override;
#endif

  void leaveEvent(QEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

private:
  QLabel* m_current_project_title{nullptr};
  QLabel* m_current_project_dir{nullptr};
  QColor m_widget_color;
  bool m_active{false};
  QString m_project_path;
};

}  // namespace saveloadproject

#endif  // SAVELOADPROJECT_SAVELOADPROJECTCORE_PROJECT_PANE_WIDGET_H_
