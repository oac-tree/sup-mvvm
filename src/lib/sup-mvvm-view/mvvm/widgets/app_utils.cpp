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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "app_utils.h"

#include "widget_utils.h"

#include <QApplication>
#include <QDebug>
#include <QFont>
#include <QGuiApplication>
#include <QScreen>
#include <QSize>
#include <QString>
#include <QStyle>
#include <QStyleFactory>

namespace
{

/**
 * @brief Return text of the header.
 * "str" will become "str       : "
 */
QString GetHeaderText(const QString& str)
{
  const int length(30);
  QString result(str);
  result.resize(length, ' ');
  result.append(": ");
  return result;
}

/**
 * @brief Generate string with environment variables, if any, related to scaling.
 */
QString GetEnvironmentInfo()
{
  QString result;
  result += GetHeaderText("QT_AUTO_SCREEN_SCALE_FACTOR")
            + qEnvironmentVariable("QT_AUTO_SCREEN_SCALE_FACTOR");
  result += "\n";
  result += GetHeaderText("QT_SCALE_FACTOR") + qEnvironmentVariable("QT_SCALE_FACTOR");
  result += "\n";
  result +=
      GetHeaderText("QT_SCREEN_SCALE_FACTORS") + qEnvironmentVariable("QT_SCREEN_SCALE_FACTORS");
  result += "\n";
  return result;
}

/**
 * @brief Generates string with system font metrics.
 */
QString GetFontInfo()
{
  QString result;
  auto size = ::mvvm::utils::SizeOfLetterM();
  result +=
      GetHeaderText("Size of letter M") + QString("(%1, %2)").arg(size.width()).arg(size.height());
  result += "\n";

  auto font = QGuiApplication::font();
  result += GetHeaderText("Default font point size")
            + QString("%1 (%2F)").arg(font.pointSize()).arg(font.pointSizeF()) + " "
            + font.defaultFamily();
  result += "\n";

  return result;
}

/**
 * @brief Generates string with screen geometry information.
 */
QString GetScreenInfo()
{
  QString result;
  auto screen = QGuiApplication::primaryScreen();
  auto size = screen->physicalSize();
  result += GetHeaderText("Screen physical size")
            + QString("(%1, %2)").arg(size.height()).arg(size.width());
  result += "\n";
  auto geometry = screen->geometry();
  result += GetHeaderText("Screen geometry")
            + QString("(%1, %2)").arg(geometry.height()).arg(geometry.width());
  result += "\n";

  result += GetHeaderText("Device pixel ratio") + QString("%1").arg(screen->devicePixelRatio());
  result += "\n";

  result += GetHeaderText("Logical dots per inch")
            + QString("%1 (%2, %3)")
                  .arg(screen->logicalDotsPerInch())
                  .arg(screen->logicalDotsPerInchX())
                  .arg(screen->logicalDotsPerInchY());
  result += "\n";

  result += GetHeaderText("Physical dots per inch")
            + QString("%1 (%2, %3)")
                  .arg(screen->physicalDotsPerInch())
                  .arg(screen->physicalDotsPerInchX())
                  .arg(screen->physicalDotsPerInchY());
  result += "\n";

  return result;
}

/**
 * @brief Generates string with list of available UI styles.
 */
QString GetStyleInfo()
{
  QString result(GetHeaderText("Available UI styles"));
  int index{0};
  result.append(QApplication::style()->objectName() + " (");
  for (const auto& style : QStyleFactory::keys())
  {
    result.append(style);
    if (index < QStyleFactory::keys().size() - 1)
    {
      result.append(", ");
    }
    ++index;
  }
  result += ")\n";

  return result;
}

/**
 * @brief Generates string with Qt version.
 */
QString GetQtVersion()
{
  const QString compile_time_version = QT_VERSION_STR;
  QString result(GetHeaderText("Qt version"));
  result.append(QString("Compile time %1, runtime %2\n").arg(compile_time_version, qVersion()));
  return result;
}

/**
 * @brief Checks if there is an attempt to scale via environment variable.
 */
bool IsAttemptToScaleViaEnvironment()
{
  return qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
         || qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
         || qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS");
}

/**
 * @brief Cleanup environment variables related to scaling.
 */
void ResetHighDpiEnvironment()
{
  qunsetenv("QT_AUTO_SCREEN_SCALE_FACTOR");
  qunsetenv("QT_SCALE_FACTOR");
  qunsetenv("QT_SCREEN_SCALE_FACTORS");
}

}  // namespace

namespace mvvm::utils
{

std::string GetDesktopInfo()
{
  const int line_length(80);
  QString result;

  result += QString(line_length, '-');
  result += "\n";
  result += GetEnvironmentInfo();
  result += GetFontInfo();
  result += GetScreenInfo();
  result += GetStyleInfo();
  result += GetQtVersion();

  result += QString(line_length, '-');
  result += "\n";

  return result.toStdString();
}

void SetupHighDpiScaling(bool use_environment_variable)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

  if (use_environment_variable)
  {
    if (IsAttemptToScaleViaEnvironment())
    {
      // Nothing to do if scaling is steared via environment variables
      qDebug() << "QT_ defined, no scale";
      return;
    }
    qDebug() << "Enable High Dpi";
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif
  }
  else
  {
    qDebug() << "Own scaling, resetting environment";
    // our GUI is perfect and knows how to scale by itself
    ResetHighDpiEnvironment();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling, true);
#else
    /* AA_DisableHighDpiScaling is deprecated */
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::Floor);
#endif
  }
}

void SetApplicationFontSize(int point_size)
{
  if (point_size <= 0)
  {
    // user didn't provide any meaningful font size from the command line
    return;
  }

  // We retrive current application font to set the size, and then reuse.
  // This will preserve all other font properties.
  auto font = QApplication::font();
  font.setPointSize(point_size);
  font.setHintingPreference(QFont::PreferVerticalHinting);
  QApplication::setFont(font);
}

}  // namespace mvvm::utils
