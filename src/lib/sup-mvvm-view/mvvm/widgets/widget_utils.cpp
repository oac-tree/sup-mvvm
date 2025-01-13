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

#include "widget_utils.h"

#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QDir>
#include <QFontMetrics>
#include <QLabel>
#include <QMainWindow>
#include <QSize>
#include <random>

namespace
{

/**
 * @brief Calculates the size of the letter `M` from the given font.
 */
QSize GetUnitSizeFromFont(const QFont& font)
{
  QFontMetrics fontMetric(font);
  auto em = fontMetric.horizontalAdvance('M');
  auto fontAscent = fontMetric.ascent();
  return {em, fontAscent};
}

/**
 * @brief Calculate the size of letter 'M' from system font.
 */
QSize FindSizeOfLetterM()
{
  if (QApplication::instance())
  {
    return GetUnitSizeFromFont(QApplication::font());
  }

  // when no QApplication is running return something for purpose of unit tests
  const int default_width = 11;
  const int default_height = 14;
  return {default_width, default_height};
}

/**
 * @brief Returns reference for M-size constant.
 */
QSize& GetGlobalSizeReference()
{
  static QSize result = FindSizeOfLetterM();
  return result;
}

const QString untitled_name = "Untitled";

}  // namespace

int RandInt(int low, int high)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> uniform_int(low, high);
  return uniform_int(gen);
}

namespace mvvm::utils
{

QColor RandomColor()
{
  auto rndm = []() -> int { return RandInt(0, 255); };
  return {rndm(), rndm(), rndm()};
}

std::string RandomNamedColor()
{
  return RandomColor().name().toStdString();
}

bool IsWindowsHost()
{
#if defined(Q_OS_WIN)
  return true;
#else
  return false;
#endif
}

bool IsMacHost()
{
#if defined(Q_OS_MAC)
  return true;
#else
  return false;
#endif
}

bool IsLinuxHost()
{
#if defined(Q_OS_LINUX)
  return true;
#else
  return false;
#endif
}

QString WithTildeHomePath(const QString& path)
{
  if (IsWindowsHost())
  {
    return path;
  }

  static const QString homePath = QDir::homePath();

  QFileInfo fi(QDir::cleanPath(path));
  QString outPath = fi.absoluteFilePath();
  if (outPath.startsWith(homePath))
  {
    outPath = QLatin1Char('~') + outPath.mid(homePath.size());
  }
  else
  {
    outPath = path;
  }
  return outPath;
}

int WidthOfLetterM()
{
  return SizeOfLetterM().width();
}

int HeightOfLetterM()
{
  return mvvm::utils::SizeOfLetterM().height();
}

QSize SizeOfLetterM()
{
  // returning copy
  return GetGlobalSizeReference();
}

void SetApplicationFont(const QFont& font)
{
  // assigning new size to our global M constant
  auto& ref = GetGlobalSizeReference();
  ref = GetUnitSizeFromFont(font);

  QApplication::setFont(font);
}

int SystemPointSize()
{
  const int default_size = 10;  // when no application is running
  return QApplication::instance() ? QApplication::font().pointSize() : default_size;
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

void ScaleLabelFont(QLabel* label, double scale, bool is_bold)
{
  QFont font = label->font();
  font.setPointSize(mvvm::utils::SystemPointSize() * scale);
  if (is_bold)
  {
    font.setBold(true);
  }
  label->setFont(font);
}

QStringList GetStringList(const std::vector<std::string>& vec)
{
  QStringList result;
  std::transform(vec.begin(), vec.end(), std::back_inserter(result),
                 [](const auto& str) { return QString::fromStdString(str); });
  return result;
}

std::vector<std::string> GetStdStringVector(const QStringList& string_list)
{
  std::vector<std::string> result;
  std::transform(string_list.begin(), string_list.end(), std::back_inserter(result),
                 [](const auto& str) { return str.toStdString(); });
  return result;
}

QByteArray GetByteArray(const QStringList& data)
{
  QByteArray byteArray;
  QDataStream out(&byteArray, QIODevice::WriteOnly);
  out << data;
  return byteArray;
}

QStringList GetStringList(const QByteArray& byteArray)
{
  QByteArray array = byteArray;
  QStringList result;
  QDataStream in(&array, QIODevice::ReadOnly);
  in >> result;
  return result;
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

int UnitSize(double scale)
{
  return static_cast<int>(std::round(WidthOfLetterM() * scale));
}

}  // namespace mvvm::utils
