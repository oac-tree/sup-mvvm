/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "mvvm/widgets/collapsible_widget.h"

#include <mvvm/widgets/collapsible_toolbar.h>

#include <QAction>
#include <QDebug>
#include <QSplitter>
#include <QToolBar>
#include <QVBoxLayout>

namespace mvvm
{

CollapsibleWidget::CollapsibleWidget(QWidget *context, QWidget *parent)
    : QWidget(parent), m_tool_bar(new CollapsibleToolBar)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(context);
  m_tool_bar->SetControlledWidget(this);
  m_tool_bar->SetText(context->windowTitle(), context->toolTip());
}

CollapsibleWidget::~CollapsibleWidget() = default;

void CollapsibleWidget::AddToSplitter(QSplitter *splitter)
{
  splitter->addWidget(m_tool_bar);
  splitter->setCollapsible(splitter->indexOf(m_tool_bar), false);

  splitter->addWidget(this);
}

CollapsibleToolBar *CollapsibleWidget::GetToolBar()
{
  return m_tool_bar;
}

}  // namespace mvvm
