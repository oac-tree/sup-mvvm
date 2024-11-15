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

#include "recent_project_settings.h"

#include <mvvm/utils/file_utils.h>

#include <QDir>
#include <QSettings>

namespace mvvm
{

namespace
{

const int kMaxProjectCount = 10;

QString GetCurrentWorkdirSettingName(const QString& group_name)
{
  return group_name + "/" + "workdir";
}

QString GetRecentProjectsSettingName(const QString& group_name)
{
  return group_name + "/" + "recent_projects";
}

}  // namespace

RecentProjectSettings::RecentProjectSettings(const QString& group_name)
    : RecentProjectPath(kMaxProjectCount), m_group_name(group_name)
{
  ReadSettings();
}

RecentProjectSettings::~RecentProjectSettings()
{
  WriteSettings();
}

void RecentProjectSettings::WriteSettings()
{
  ValidateIfProjectsExist();

  QSettings settings;
  settings.setValue(GetCurrentWorkdirSettingName(m_group_name), GetCurrentWorkdir());
  settings.setValue(GetRecentProjectsSettingName(m_group_name), GetRecentProjectList());
}

void RecentProjectSettings::ReadSettings()
{
  ValidateIfProjectsExist();

  const QSettings settings;
  SetCurrentWorkdir(
      settings.value(GetCurrentWorkdirSettingName(m_group_name), QDir::homePath()).toString());

  SetRecentProjectList(settings.value(GetRecentProjectsSettingName(m_group_name)).toStringList());
}

}  // namespace mvvm
