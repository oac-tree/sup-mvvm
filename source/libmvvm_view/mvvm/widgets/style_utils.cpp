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

#include "mvvm/widgets/style_utils.h"

#include <mvvm/widgets/widget_utils.h>

#include <QApplication>
#include <QIcon>
#include <QSize>
#include <QStyle>
#include <QTreeView>
#include <cmath>

namespace
{
//! Flag if to use svg versions of icons.
const bool kSvgIcons = false;

QString CreatePopertyTreeStyleString()
{
  QString result = R"(
  QTreeView::branch:has-siblings:!adjoins-item {
      border-image: url(:/icons/vline.png) 0;
  }

  QTreeView::branch:has-siblings:adjoins-item {
      border-image: url(:/icons/branch-more.png) 0;
  }

  QTreeView::branch:!has-children:!has-siblings:adjoins-item {
      border-image: url(:/icons/branch-end.png) 0;
  }

  QTreeView::branch:has-children:!has-siblings:closed,
  QTreeView::branch:closed:has-children:has-siblings {
          padding:2px 2px 2px 2px;border-image: none;
          image: url(:/icons/chevron-right.png);
  }

  QTreeView::branch:open:has-children:!has-siblings,
  QTreeView::branch:open:has-children:has-siblings  {
          padding:2px 2px 2px 2px;border-image: none;
          image: url(:/icons/chevron-down.png);
  })";

  return result;
}

}  // namespace

namespace mvvm::utils
{

QSize ToolBarIconSize()
{
  const int width = UnitSize(2.3);
  return {width, width};
}

QSize NarrowToolBarIconSize()
{
  const int width = std::round(UnitSize(1.75));
  return {width, width};
}

QIcon GetIcon(const std::string &icon_name)
{
  auto name = QString(":/icons/%1").arg(QString::fromStdString(icon_name));
  if (!kSvgIcons)
  {
    name.replace(".svg", ".png");
  }
  return QIcon(name);
}

void SetBreezePropertyStyle(QTreeView *tree)
{
  if (QApplication::style()->objectName() != QString("breeze"))
  {
    tree->setStyleSheet(CreatePopertyTreeStyleString());
  }
}

}  // namespace mvvm::utils
