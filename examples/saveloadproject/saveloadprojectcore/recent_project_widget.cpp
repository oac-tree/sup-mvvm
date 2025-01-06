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

#include "recent_project_widget.h"

#include "project_pane_widget.h"

#include <mvvm/widgets/adjusting_scroll_area.h>
#include <mvvm/widgets/widget_utils.h>

#include <QLabel>
#include <QVBoxLayout>

namespace saveloadproject
{

namespace
{
int kMaxRecentProjectCount = 7;
}

RecentProjectWidget::RecentProjectWidget(QWidget* parent)
    : QWidget(parent), m_current_project_pane(new ProjectPaneWidget)
{
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(20, 0, 10, 0);
  layout->addWidget(CreateRecentProjectScrollArea());
  layout->addStretch(1);
}

void RecentProjectWidget::UpdateCurrentProjectInfo(const QString& path, bool is_modified)
{
  m_current_project_pane->SetProjectInfo(path, is_modified);
  m_current_project_pane->SetActive(false);
}

void RecentProjectWidget::UpdateRecentProjectsList(const QStringList& paths)
{
  int widget_index{0};
  for (auto widget : m_recent_project_panes)
  {
    if (widget_index < paths.size())
    {
      widget->SetProjectInfo(paths.at(widget_index), false);
    }
    else
    {
      widget->Clear();
    }

    ++widget_index;
  }
}

QSize RecentProjectWidget::sizeHint() const
{
  const auto panel_width = mvvm::utils::UnitSize(32);
  return {panel_width, panel_width * 2};
}

QSize RecentProjectWidget::minimumSizeHint() const
{
  const auto minimum_panel_width = mvvm::utils::UnitSize(16);
  return {minimum_panel_width, minimum_panel_width * 2};
}

QBoxLayout* RecentProjectWidget::CreateCurrentProjectLayout() const
{
  auto result = new QVBoxLayout;
  auto label = new QLabel("Current Project");
  result->addWidget(label);
  result->addWidget(m_current_project_pane);
  return result;
}

QBoxLayout* RecentProjectWidget::CreateRecentProjectLayout()
{
  auto result = new QVBoxLayout;
  auto label = new QLabel("Recent Projects");
  result->addWidget(label);

  for (int i = 0; i < kMaxRecentProjectCount; ++i)
  {
    auto widget = new ProjectPaneWidget;
    connect(widget, &ProjectPaneWidget::projectSelected, this,
            &RecentProjectWidget::projectSelected);
    m_recent_project_panes.push_back(widget);
    result->addWidget(widget);
  }
  return result;
}

QWidget* RecentProjectWidget::CreateRecentProjectScrollArea()
{
  auto result = new mvvm::AdjustingScrollArea;

  auto content = new QWidget;
  auto layout = new QVBoxLayout;
  layout->addLayout(CreateCurrentProjectLayout());
  layout->addSpacing(mvvm::utils::UnitSize());
  layout->addLayout(CreateRecentProjectLayout());
  content->setLayout(layout);

  result->setWidget(content);
  return result;
}

}  // namespace saveloadproject
