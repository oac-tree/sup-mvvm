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

#include "project_utils.h"

#include "project.h"

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/project/project_interface.h>
#include <mvvm/utils/file_utils.h>

#include <algorithm>
#include <cctype>

namespace
{
const std::string kXMLExtension = ".xml";
const std::string kUntitledName = "Untitled";
const std::string kDefaultModelName = "application_model";
}  // namespace

namespace mvvm::utils
{

std::string SuggestFileName(const SessionModelInterface& model)
{
  std::string result = model.GetType().empty() ? kDefaultModelName : model.GetType();
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result + kXMLExtension;
}

bool IsPossibleProjectDir(const std::string& project_dir)
{
  return !utils::FindFiles(project_dir, kXMLExtension).empty();
}

std::unique_ptr<ProjectInterface> CreateUntitledProject(const ProjectContext& context)
{
  return std::make_unique<Project>(context);
}

std::string ProjectWindowTitle(const ProjectInterface& project)
{
  return ProjectWindowTitle(project.GetProjectDir(), project.IsModified());
}

std::string ProjectWindowTitle(const std::string& project_dir, bool is_modified)
{
  auto pos = project_dir.find_last_of('/');
  auto project_name = (pos == std::string::npos ? kUntitledName : project_dir.substr(pos + 1));
  auto unsaved_status = is_modified ? "*" : "";
  return unsaved_status + project_name;
}

}  // namespace mvvm::utils
