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

#include "mvvm/widgets/mainbar_widget.h"

#include <mvvm/widgets/fancytab.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace mvvm
{
MainBarWidget::MainBarWidget(QWidget* parent)
    : QFrame(parent), m_stacked_widget(new QStackedWidget), m_label_layout(new QHBoxLayout)
{
  m_label_layout->setContentsMargins(2, 2, 2, 2);

  auto layout = new QVBoxLayout(this);
  layout->addLayout(m_label_layout);
  layout->addWidget(m_stacked_widget);
  layout->setContentsMargins(0, 0, 0, 0);

  setFrameStyle(QFrame::Sunken);
}

MainBarWidget::~MainBarWidget() = default;

void MainBarWidget::addWidget(QWidget* widget, const QString& title, bool is_enabled)
{
  int index = m_stacked_widget->addWidget(widget);

  auto tab = new FancyTab(title);
  tab->setEnabled(is_enabled);
  auto on_tab_clicked = [this, index]() { setCurrentIndex(index); };
  connect(tab, &FancyTab::clicked, on_tab_clicked);

  m_index_to_tab[index] = tab;
  m_label_layout->addWidget(tab);
}

void MainBarWidget::setCurrentIndex(int index)
{
  for (auto it : m_index_to_tab)
  {
    it.second->setSelected(it.first == index);
  }

  m_stacked_widget->setCurrentIndex(index);
}

}  // namespace mvvm
