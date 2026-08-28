/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "widget_utils.h"

#include <mvvm/core/platform.h>
#include <mvvm/utils/numeric_utils.h>

#include <QApplication>
#include <QColor>
#include <QDir>
#include <QMainWindow>

namespace mvvm::utils
{

QColor RandomColor()
{
  auto rndm = []() -> std::int32_t { return RandInt(0, 255); };
  return {rndm(), rndm(), rndm()};
}

std::string RandomNamedColor()
{
  return RandomColor().name().toStdString();
}

QString CollapseHomePath(const QString& path)
{
  if (IsWindowsHost())
  {
    return path;
  }

  static const QString home_path = QDir::homePath();

  const QFileInfo file_info(QDir::cleanPath(path));
  QString out_path = file_info.absoluteFilePath();
  if (out_path.startsWith(home_path))
  {
    out_path = QLatin1Char('~') + out_path.mid(home_path.size());
  }
  else
  {
    out_path = path;
  }
  return out_path;
}

QString ExpandHomePath(const QString& path)
{
  if (IsWindowsHost())
  {
    return path;
  }

  static const QString home_path = QDir::homePath();

  if (path == QLatin1String("~"))
  {
    return home_path;
  }

  if (path.startsWith(QLatin1String("~/")))
  {
    return home_path + path.mid(1);
  }

  return path;
}

QMainWindow* FindMainWindow()
{
  for (auto widget : qApp->topLevelWidgets())
  {
    if (auto result = dynamic_cast<QMainWindow*>(widget); result)
    {
      return result;
    }
  }
  return nullptr;
}

QString ClickableText(const QString& text, const QString& link)
{
  return QString(R"(<a href="%1">%2</a>)").arg(link.isEmpty() ? text : link, text);
}

QString CreatePathPresentation(const QString& text)
{
  if (text.isEmpty())
  {
    return {};
  }

  if (text == QStringLiteral("/"))
  {
    return ClickableText("/", "/");
  }
  auto parts = text.split(QLatin1Char('/'));

  QString link;    //! real link to follow
  QString result;  //! text for qlabel
  for (auto it = parts.begin(); it != parts.end(); ++it)
  {
    if (it->isEmpty())
    {
      link.append(QString("/"));
      result.append(ClickableText("/ ", link));
      continue;
    }

    link.append(*it);
    result.append(ClickableText(*it, link));
    if (std::next(it) != parts.end())
    {
      link.append(QString("/"));
      result.append(QString(" / "));
    }
  }
  return result;
}

}  // namespace mvvm::utils
