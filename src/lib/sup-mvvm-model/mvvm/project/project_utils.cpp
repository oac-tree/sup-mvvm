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

#include "mvvm/project/project_utils.h"

#include <mvvm/interfaces/project_interface.h>
#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/project/project.h>
#include <mvvm/utils/file_utils.h>

#include <cctype>

namespace
{
const std::string xml_extention = ".xml";
const std::string untitled_name = "Untitled";
}  // namespace

namespace mvvm
{

std::string ProjectUtils::SuggestFileName(const SessionModelInterface& model)
{
  std::string result = model.GetType();
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result + xml_extention;
}

bool ProjectUtils::IsPossibleProjectDir(const std::string& project_dir)
{
  return !utils::FindFiles(project_dir, xml_extention).empty();
}

std::unique_ptr<ProjectInterface> ProjectUtils::CreateUntitledProject(const ProjectContext& context)
{
  return std::make_unique<Project>(context);
}

std::string ProjectUtils::ProjectWindowTitle(const ProjectInterface& project)
{
  return ProjectWindowTitle(project.GetProjectDir(), project.IsModified());
}

std::string ProjectUtils::ProjectWindowTitle(const std::string& project_dir, bool is_modified)
{
  auto pos = project_dir.find_last_of('/');
  auto project_name = (pos == std::string::npos ? untitled_name : project_dir.substr(pos + 1));
  auto unsaved_status = is_modified ? "*" : "";
  return unsaved_status + project_name;
}

}  // namespace mvvm
