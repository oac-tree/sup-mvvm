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

#include "project_pane_widget.h"

#include <mvvm/project/project_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>

namespace saveloadproject
{

namespace
{

int GetWidgetHeight()
{
  return mvvm::utils::UnitSize(4.5);
}

}  // namespace

ProjectPaneWidget::ProjectPaneWidget(QWidget* parent)
    : QWidget(parent)
    , m_current_project_title(new QLabel(" "))
    , m_current_project_dir(new QLabel(" "))
    , m_widget_color(QColor(Qt::white))
{
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  setFixedHeight(GetWidgetHeight());
  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_current_project_title);
  layout->addWidget(m_current_project_dir);
}

void ProjectPaneWidget::SetProjectInfo(const QString& path, bool is_modified)
{
  m_active = true;
  m_project_path = path;

  auto trimmed_project_path = mvvm::utils::WithTildeHomePath(path);
  auto project_title = mvvm::utils::ProjectWindowTitle(path.toStdString(), is_modified);

  m_current_project_dir->setText(trimmed_project_path);
  m_current_project_dir->setToolTip(m_project_path);
  m_current_project_title->setText(QString::fromStdString(project_title));
}

void ProjectPaneWidget::Clear()
{
  SetActive(false);
  m_project_path.clear();
  m_current_project_dir->setText({});
  m_current_project_dir->setToolTip({});
  m_current_project_title->setText({});
}

void ProjectPaneWidget::SetActive(bool value)
{
  m_active = value;
  update();
}

void ProjectPaneWidget::paintEvent(QPaintEvent* event)
{
  (void)event;
  QPainter painter(this);
  painter.fillRect(0, 0, size().width(), size().height(), m_widget_color);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void ProjectPaneWidget::enterEvent(QEvent* event)
#else
void ProjectPaneWidget::enterEvent(QEnterEvent* event)
#endif
{
  (void)event;

  if (m_active)
  {
    m_widget_color = QColor(Qt::lightGray);
  }
  update();
}

void ProjectPaneWidget::leaveEvent(QEvent* event)
{
  (void)event;
  m_widget_color = QColor(Qt::white);
  update();
}

void ProjectPaneWidget::mousePressEvent(QMouseEvent* event)
{
  if (m_active && event->button() == Qt::LeftButton)
  {
    emit projectSelected(m_project_path);
  }
}

}  // namespace saveloadproject
