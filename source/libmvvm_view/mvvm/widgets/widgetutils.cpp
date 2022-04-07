/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "widgetutils.h"

#include <QApplication>
#include <QColor>
#include <QDir>
#include <QFontMetrics>
#include <QLabel>
#include <QMainWindow>
#include <QSize>
#include <random>

namespace
{
//! Calculates size of letter `M` for current system font settings.

QSize FindSizeOfLetterM()
{
  QFontMetrics fontMetric(QApplication::font());
  auto em = fontMetric.horizontalAdvance('M');
  auto fontAscent = fontMetric.ascent();
  return QSize(em, fontAscent);
}

const QString untitled_name = "Untitled";

}  // namespace

int mvvm::utils::RandInt(int low, int high)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> uniform_int(low, high);
  return uniform_int(gen);
}

QColor mvvm::utils::RandomColor()
{
  auto rndm = []() -> int { return mvvm::utils::RandInt(0, 255); };
  return QColor(rndm(), rndm(), rndm());
}

std::string mvvm::utils::RandomNamedColor()
{
  return RandomColor().name().toStdString();
}

bool mvvm::utils::IsWindowsHost()
{
#if defined(Q_OS_WIN)
  return true;
#else
  return false;
#endif
}

bool mvvm::utils::IsMacHost()
{
#if defined(Q_OS_MAC)
  return true;
#else
  return false;
#endif
}

bool mvvm::utils::IsLinuxHost()
{
#if defined(Q_OS_LINUX)
  return true;
#else
  return false;
#endif
}

QString mvvm::utils::WithTildeHomePath(const QString& path)
{
  if (mvvm::utils::IsWindowsHost())
  {
    return path;
  }

  static const QString homePath = QDir::homePath();

  QFileInfo fi(QDir::cleanPath(path));
  QString outPath = fi.absoluteFilePath();
  if (outPath.startsWith(homePath))
    outPath = QLatin1Char('~') + outPath.mid(homePath.size());
  else
    outPath = path;
  return outPath;
}

//! Project without projectDir will be "Untitled", modified project will be "*Untitled".
//! Project with projectDir in "/home/user/project1" will get title "project1".

QString mvvm::utils::ProjectWindowTitle(const QString& project_dir, bool is_modified)
{
  auto pos = project_dir.lastIndexOf('/');
  auto project_name = (pos == -1) ? untitled_name : project_dir.mid(pos + 1);
  auto unsaved_status = is_modified ? QString("*") : QString();
  return unsaved_status + project_name;
}

int mvvm::utils::WidthOfLetterM()
{
  return mvvm::utils::SizeOfLetterM().width();
}

int mvvm::utils::HeightOfLetterM()
{
  return mvvm::utils::SizeOfLetterM().height();
}

QSize mvvm::utils::SizeOfLetterM()
{
  static QSize result = FindSizeOfLetterM();
  return result;
}

int mvvm::utils::SystemPointSize()
{
  return QApplication::font().pointSize();
}

QMainWindow* mvvm::utils::FindMainWindow()
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

QString mvvm::utils::ClickableText(const QString& text, const QString& tag)
{
  return QString(R"(<a href="%1">%2</a>)").arg(tag.isEmpty() ? text : tag, text);
}

void mvvm::utils::ScaleLabelFont(QLabel* label, double scale)
{
  QFont font = label->font();
  font.setPointSize(mvvm::utils::SystemPointSize() * scale);
  label->setFont(font);
}

QStringList mvvm::utils::GetStringList(const std::vector<std::string>& vec)
{
  QStringList result;
  for (const auto& x : vec)
  {
    result.push_back(QString::fromStdString(x));
  }
  return result;
}

std::vector<std::string> mvvm::utils::GetStdStringVector(const QStringList& string_list)
{
  std::vector<std::string> result;
  for (const auto& x : string_list)
  {
    result.push_back(x.toStdString());
  }
  return result;
}

QByteArray mvvm::utils::GetByteArray(const QStringList& data)
{
  QByteArray byteArray;
  QDataStream out(&byteArray, QIODevice::WriteOnly);
  out << data;
  return byteArray;
}

QStringList mvvm::utils::GetStringList(const QByteArray& byteArray)
{
  QByteArray array = byteArray;
  QStringList result;
  QDataStream in(&array, QIODevice::ReadOnly);
  in >> result;
  return result;
}

QString mvvm::utils::CreatePathPresentation(const QString& text)
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

  QString link; //! real link to follow
  QString result; //! text for qlabel
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
